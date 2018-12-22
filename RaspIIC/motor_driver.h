#ifndef __MOTOR_DRIVER_H
#define __MOTOR_DRIVER_H

#include "pwmserv_driver.h"

#define MICROSTEPS 8

#define	FORWARD		1
#define	BACKWARD	2
#define	BRAKE		3
#define	RELEASE		4

#define	SINGLE		1
#define	DOUBLE		2
#define	INTERLEAVE	3
#define	MICROSTEP	4

class MotorHAT;

//步进电机
class StepperMotor
{
private:
	int PWMA;
	int AIN2;
	int AIN1;
	int PWMB;
	int BIN2;
	int BIN1;
	MotorHAT * MC;
	int revsteps;
	int motornum;
	float sec_per_step;
	int steppingcounter;
	int currentstep;

public:
	StepperMotor();
	void init(int num, MotorHAT * mc,int steps=200);
	void setSpeed(int rpm);
	int oneStep(int dir, int style);
	void step(int steps, int direction, int stepstyle);
};

//直流电机
class DCMotor
{
private:
	int motornum;
	MotorHAT * MC;
	int PWMpin;
	int IN1pin;
	int IN2pin;
	
public:
	DCMotor();
	void init(int num, MotorHAT * mc);
	void  run(int command);
	void setSpeed(int speed);
};

//舵机
class Servo
{
private:
 	int servonum;
	MotorHAT * MC;
	
public:
	Servo();
	void init(int num, MotorHAT * mc);
	void write(int x);
	void setServoPulse(int channel, float pulse);
};

//控制器
class MotorHAT
{
private:
 	int _i2caddr;
	int _frequency;
	DCMotor motors[4];
	StepperMotor steppers[2];
	Servo servos[4];
	
public:
	PWM _pwm;
	MotorHAT(int addr, int freq);
	void setPin(int pin, int value);
	StepperMotor & getStepper(int num);
	DCMotor & getMotor(int num);
	Servo & getServo(int num);
};

#endif
