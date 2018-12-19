#include <iostream>

#include "motor_driver.h"
#include "pwmserv_driver.h"

#if (MICROSTEPS == 8)
unsigned char MICROSTEP_CURVE[] = {0, 50, 98, 142, 180, 212, 236, 250, 255};
#elif (MICROSTEPS == 16)
unsigned char MICROSTEP_CURVE[] = {0, 25, 50, 74, 98, 120, 141, 162, 180, 197, 212, 225, 236, 244, 250, 253, 255};
#endif


StepperMotor::StepperMotor(int num, int steps)
{
	MC = NULL;
	revsteps = steps;
	motornum = num;
	sec_per_step = 0.1;
	steppingcounter = 0;
	currentstep = 0;

	num -= 1;

	if (num == 0)
	{
		PWMA = 8;
		AIN2 = 9;
		AIN1 = 10;
		PWMB = 13;
		BIN2 = 12;
		BIN1 = 11;
	}
	else if (num == 1)
	{
		PWMA = 2;
		AIN2 = 3;
		AIN1 = 4;
		PWMB = 7;
		BIN2 = 6;
		BIN1 = 5;
	}
	else
	{
		cout << "Motor Stepper must be between 1 and 2 inclusive, EXIT!" << endl;
		exit(0);
	}
}

void StepperMotor::setSpeed(int rpm)
{
	sec_per_step = 60.0/(revsteps * rpm);
	steppingcounter = 0;
}

int StepperMotor::oneStep(int dir, int style)
{
	int pwm_a = 255;
	int pwm_b = 255;

	//first determine what sort of stepping procedure we're up to
	if (style == SINGLE)
	{
		if ((currentstep/(MICROSTEPS/2)) % 2)
		{
			//we're at an odd step, weird
			if(dir == FORWARD)
			{
				currentstep += MICROSTEPS/2;
			}
			else
			{
				currentstep -= MICROSTEPS/2;
			}
		}
		else
		{
			//go to next even step
			if (dir == FORWARD)
			{
				currentstep += MICROSTEPS;
			}
			else
			{
				currentstep -= MICROSTEPS;
			}
		}
	}
	if(style == DOUBLE)
	{
		if(!(currentstep/(MICROSTEPS/2) % 2))
		{
			//we're at an even step, weird
			if (dir == FORWARD)
			{
				currentstep += MICROSTEPS/2;
			}
			else
			{
				currentstep -= MICROSTEPS/2;
			}
		}
		else
		{
			//go to next odd step
			if (dir == FORWARD)
			{
				currentstep += MICROSTEPS;
			}
			else
			{
				currentstep -= MICROSTEPS;
			}
		}
	}
	if (style == INTERLEAVE)
	{
		if (dir == FORWARD)
		{
			currentstep += MICROSTEPS/2;
		}
		else
		{
			currentstep -= MICROSTEPS/2;
		}
	}

	if (style == MICROSTEP)
	{
		if (dir == FORWARD)
		{
			currentstep += 1;
		}
		else
		{
			currentstep -= 1;
        	//go to next 'step' and wrap around
        	currentstep += MICROSTEPS * 4;
        	currentstep %= MICROSTEPS * 4;
		}

		pwm_a = 0;
		pwm_b = 0;
		if((currentstep >= 0) && (currentstep < MICROSTEPS))
		{
			pwm_a = MICROSTEP_CURVE[MICROSTEPS - currentstep];
			pwm_b = MICROSTEP_CURVE[currentstep];
		}
		else if((currentstep >= MICROSTEPS) && (currentstep < MICROSTEPS*2))
		{
			pwm_a = MICROSTEP_CURVE[currentstep - MICROSTEPS];
			pwm_b = MICROSTEP_CURVE[MICROSTEPS*2 - currentstep];
		}
		else if((currentstep >= MICROSTEPS*2) && (currentstep < MICROSTEPS*3))
		{
			pwm_a = MICROSTEP_CURVE[MICROSTEPS*3 - currentstep];
			pwm_b = MICROSTEP_CURVE[currentstep - MICROSTEPS*2];
		}
		else if((currentstep >= MICROSTEPS*3) && (currentstep < MICROSTEPS*4))
		{
            pwm_a = MICROSTEP_CURVE[currentstep - MICROSTEPS*3];
            pwm_b = MICROSTEP_CURVE[MICROSTEPS*4 - currentstep];
		}
	}

	//go to next 'step' and wrap around
	currentstep += MICROSTEPS * 4;
	currentstep %= MICROSTEPS * 4;

	//only really used for microstepping, otherwise always on!
	MC->_pwm.setPWM(PWMA, 0, pwm_a*16);
	MC->_pwm.setPWM(PWMB, 0, pwm_b*16);

	//set up coil energizing!
	int coils[4] = {0, 0, 0, 0};

	if (style == MICROSTEP)
	{
		if((currentstep >= 0) &&(currentstep < MICROSTEPS))
		{
			coils = {1, 1, 0, 0};
		}
        else if((currentstep >= MICROSTEPS) && (currentstep < MICROSTEPS*2))
        {
			coils = {0, 1, 1, 0};
        }
		else if((currentstep >= MICROSTEPS*2) && (currentstep < MICROSTEPS*3))
		{
			coils = {0, 0, 1, 1};
		}
        else if((currentstep >= MICROSTEPS*3) && (currentstep < MICROSTEPS*4))
        {
			coils = {1, 0, 0, 1};
		}
	}
	else
	{
		int step2coils[8][4] = {
			{1, 0, 0, 0}, 
			{1, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 1, 1, 0},
			{0, 0, 1, 0},
			{0, 0, 1, 1},
			{0, 0, 0, 1},
			{1, 0, 0, 1}};
		coils = step2coils[currentstep/(MICROSTEPS/2)];
	}

	//print "coils state = " + str(coils)
	MC.setPin(AIN2, coils[0]);
	MC.setPin(BIN1, coils[1]);
	MC.setPin(AIN1, coils[2]);
	MC.setPin(BIN2, coils[3]);

	return currentstep;
}

void StepperMotor::step(int steps, int direction, int stepstyle)
{
	float s_per_s = sec_per_step;
	int lateststep = 0;
	
	if (stepstyle == INTERLEAVE)
	{
		s_per_s = s_per_s / 2.0;
	}
	if (stepstyle == MICROSTEP)
	{
		s_per_s /= MICROSTEPS;
		steps *= MICROSTEPS;
	}

	cout << "Stepper:" << s_per_s << " sec per step" << endl;

	for(int i=0; i<steps; i++)
	{
		lateststep = oneStep(direction, stepstyle);
		sleep(s_per_s);
	}

	if(stepstyle == MICROSTEP)
	{
		//this is an edge case, if we are in between full steps, lets just keep going
		//so we end on a full step
		while((lateststep != 0) && (lateststep != MICROSTEPS))
		{
			lateststep = oneStep(direction, stepstyle);
			sleep(s_per_s);
		}
	}
}

//ֱ�����
DCMotor::DCMotor(int num)
{
	MC = NULL;
	motornum = num;
    int pwm = 0;
	int in1 = 0;
	int in2 = 0;

    if (num == 0)
    {
		pwm = 8;
		in2 = 9;
		in1 = 10;
    }
    else if (num == 1)
    {
		pwm = 13;
		in2 = 12;
		in1 = 11;
    }
    else if (num == 2)
    {
		pwm = 2;
		in2 = 3;
		in1 = 4;
    }
    else if(num == 3)
    {
		pwm = 7;
		in2 = 6;
		in1 = 5;
    }
	else
	{
		cout << "MotorHAT Motor must be between 1 and 4 inclusive" << endl;
		exit(0);
	}
	PWMpin = pwm;
	IN1pin = in1;
	IN2pin = in2;
}

void  DCMotor::run(int command)
{
	if(MC == NULL)
	{
		return;
	}
	if (command == FORWARD)
	{
		MC.setPin(IN2pin, 0);
		MC.setPin(IN1pin, 1);
	}
	if(command == BACKWARD)
	{
		MC.setPin(IN1pin, 0);
		MC.setPin(IN2pin, 1);
	}
	if (command == RELEASE)
	{
		MC.setPin(IN1pin, 0);
		MC.setPin(IN2pin, 0);
	}
}

void DCMotor::setSpeed(int speed)
{
	if(speed < 0)
	{
		speed = 0;
	}
	if(speed > 255)
	{
		speed = 255;
	}
	MC._pwm.setPWM(PWMpin, 0, speed*16);
}

//���
Servo::Servo(int num)
{
	MC = NULL;
	servonum = num;
}

void Servo::write(int x)
{
    float y=x/90.0+0.6;
    printf("AAAA x:%d,y:%f",x,y);
    y=y>0,6?y:0.6;
    y=y<2.5?y:2.5;
    printf("BBBB x:%d,y:%f",x,y);
    setServoPulse(servonum,y);
}

void Servo::setServoPulse(int channel, float pulse)
{
    float pulseLength = 1000000.0;                   //1,000,000 us per second
    printf("%d fre",MC._frequency);
    pulseLength /= MC._frequency;         //60 Hz
    printf("%d us per period",pulseLength);
    pulseLength /= 4096.0;                     //12 bits of resolution
    printf("%f us per bit",pulseLength);
    pulse *= 1000.0;
    pulse /= (pulseLength*1.0)
    printf("%f pulse",pulse);
    MC._pwm.setPWM(channel, 0, int(pulse));
}

//������
MotorHAT::MotorHAT(int addr, int freq)
{
	_i2caddr = addr;		//default addr on HAT
	_frequency = freq;		//default @1600Hz PWM freq
	motors[0] = DCMotor::DCMotor(this,0);
	motors[1] = DCMotor::DCMotor(this,1);
	motors[2] = DCMotor::DCMotor(this,2);
	motors[3] = DCMotor::DCMotor(this,3);
	steppers[0] = StepperMotor::StepperMotor(this,1);
	steppers[1] = StepperMotor::StepperMotor(this,2);
	servos[0]= Servo::Servo(this,0);
	servos[1]= Servo::Servo(this,1);
	servos[2= Servo::Servo(this,14);
	servos[3]= Servo::Servo(this,15);
	_pwm = PWM::PWM(addr, false);
	_pwm.setPWMFreq(_frequency);
}

void MotorHAT::setPin(int pin, int value)
{
	if((pin < 0) || (pin > 15))
	{
		cout << "PWM pin must be between 0 and 15 inclusive,EXIT!" << endl;
		exit(0);
	}
	if((value != 0) || (value != 1))
	{
		cout << "Pin value must be 0 or 1! EXIT!" << endl;
		exit(0);
	}
	if (value == 0)
	{
		_pwm.setPWM(pin, 0, 4096);
	}
	if(value == 1)
	{
		_pwm.setPWM(pin, 4096, 0);
	}
}

StepperMotor & MotorHAT::getStepper(int num)
{
	if((num < 1) || (num > 2))
	{
    	cout << "MotorHAT Stepper must be between 1 and 2 inclusive" << endl;
		exit();
	}
	return steppers[num-1];
}

DCMotor & MotorHAT::getMotor(int num)
{
	if((num < 1) || (num > 4))
	{
		cout << "MotorHAT Motor must be between 1 and 4 inclusive" << endl;
		exit();
	}
	return motors[num-1];
}

Servo & MotorHAT::getServo(int num)
{
	if((num!=0) && (num!=1) && (num!=14) && (num!=15))
	{
		cout << "MotorHAT Servo must be [0,1,14,15]" << endl;
		exit(0);
	}
	if((num == 14) || (num == 15))
	{
		num=num-12;
	}
	return servos[num];
}