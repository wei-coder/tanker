#ifndef __TANKER_H
#define __TANKER_H

#include "motor_driver.h"

#define TURN_TIME	1
#define TURN_SPEED	1
#define DEFAULT_SPEED	1

class Tanker
{
private:
	DCMotor & lmotor;
	DCMotor & rmotor;
public:
	Tanker(DCMotor & left,DCMotor & right);
	void left(int speed);
	void right(int speed);
	void backward(int speed);
	void forward(int speed);
	void stop();
	void moveto(int x,int y,int speed);
};

#endif
