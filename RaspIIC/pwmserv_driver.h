
#ifndef __PWMSERV_DRIVER_H_
#define __PWMSERV_DRIVER_H_

#include "iicdriver.h"

// Registers/etc.
#define	__MODE1				0x00
#define	__MODE2 			0x01
#define	__SUBADR1			0x02
#define	__SUBADR2			0x03
#define	__SUBADR3			0x04
#define	__PRESCALE			0xFE
#define	__LED0_ON_L 		0x06
#define	__LED0_ON_H 		0x07
#define	__LED0_OFF_L		0x08
#define	__LED0_OFF_H		0x09
#define	__ALL_LED_ON_L		0xFA
#define	__ALL_LED_ON_H		0xFB
#define	__ALL_LED_OFF_L 	0xFC
#define	__ALL_LED_OFF_H 	0xFD

//Bits
#define	__RESTART			0x80
#define	__SLEEP 			0x10
#define	__ALLCALL			0x01
#define	__INVRT 			0x10
#define	__OUTDRV			0x04

class PWM
{

public:
  I2C_dev iic;
  int	addr;
  bool	debug;

  void softwareReset(I2C_dev & cls);
  PWM(int address=0x40, bool debug=false);
  void setPWMFreq(int freq);
  void setPWM(int channel, int on, int off);
  void setAllPWM(int on, int off);
};

#endif
