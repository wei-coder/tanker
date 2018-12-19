
#include <ctime>
#include "iicdriver.h"
#include "pwmserv_driver.h"

void softwareReset(I2C_dev cls)
{
	//"Sends a software reset (SWRST) command to all the servo drivers on the bus"
	cls.writeRaw8(0x06);
}

PWM::PWM(int address, bool debug)
{
	iic = I2C_dev(address);
	iic.debug = debug;
	addr = address;
	this->debug = debug;
	if (this->debug)
	{
		cout << "Reseting PCA9685 MODE1 (without SLEEP) and MODE2" << endl;
	}
	setAllPWM(0, 0);
	iic.write8(__MODE2, __OUTDRV);
	iic.write8(__MODE1, __ALLCALL);
	usleep(5000);					//wait for oscillator

	int mode1 = iic.readU8(__MODE1);
	mode1 = mode1 & ~__SLEEP;                 //wake up (reset sleep)
	iic.write8(__MODE1, mode1);
	usleep(5000);                             //wait for oscillator
}

void PWM::setPWMFreq(int freq)
{
	//"Sets the PWM frequency"
	float prescaleval = 25000000.0;    // 25MHz
	prescaleval /= 4096.0;       // 12-bit
	prescaleval /= float(freq);
	prescaleval -= 1.0;
	if(this->debug)
	{
	  cout << "Setting PWM frequency to " << freq <<" Hz" << endl;
	  cout << "Estimated pre-scale: " << prescaleval << endl;
	}
	int prescale = (int)(prescaleval + 0.5);
	if (this->debug)
	{
	  cout << "Final pre-scale: " << prescale << endl;
	}

	int oldmode = iic.readU8(__MODE1);
	int newmode = (oldmode & 0x7F) | 0x10;			//sleep
	iic.write8(__MODE1, newmode)        			//go to sleep
	iic.write8(__PRESCALE, prescale);
	iic.write8(__MODE1, oldmode);
	usleep(5000);
	iic.write8(__MODE1, oldmode | 0x80);
}

void PWM::setPWM(int channel, int on, int off)
{
	//"Sets a single PWM channel"
	iic.write8(__LED0_ON_L+4*channel, on & 0xFF);
	iic.write8(__LED0_ON_H+4*channel, on >> 8);
	iic.write8(__LED0_OFF_L+4*channel, off & 0xFF);
	iic.write8(__LED0_OFF_H+4*channel, off >> 8);
}

void PWM::setAllPWM(int on, int off)
{
	//"Sets a all PWM channels"
	iic.write8(__ALL_LED_ON_L, on & 0xFF);
	iic.write8(__ALL_LED_ON_H, on >> 8);
	iic.write8(__ALL_LED_OFF_L, off & 0xFF);
	iic.write8(__ALL_LED_OFF_H, off >> 8);
}

