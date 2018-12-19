#include "tanker.h"

Tanker::Tanker(DCMotor & left,DCMotor & right)
{
	lmotor = left;
	rmotor = right;
}

void Tanker::left(int speed = TURN_SPEED)
{
	rmotor.setSpeed(speed);
	lmotor.setSpeed(speed);
	rmotor.run(FORWARD);
	lmotor.run(BACKWARD);
}

void Tanker::right(int speed = TURN_SPEED)
{
	rmotor.setSpeed(speed);
	lmotor.setSpeed(speed);
	rmotor.run(BACKWARD);
	lmotor.run(FORWARD);
}

void Tanker::backward(int speed = DEFAULT_SPEED)
{
	rmotor.setSpeed(speed);
	lmotor.setSpeed(speed);
	rmotor.run(FORWARD);
	lmotor.run(FORWARD);
}

void Tanker::forward(int speed = DEFAULT_SPEED)
{
	rmotor.setSpeed(speed);
	lmotor.setSpeed(speed);
	rmotor.run(BACKWARD);
	lmotor.run(BACKWARD);
}

void Tanker::moveto(int x,int y, int speed = DEFAULT_SPEED)
{
	unsigned int t = 1;
	rmotor.setSpeed(speed);
	lmotor.setSpeed(speed);
	if(x>0)
	{
		rmotor.run(BACKWARD);
		lmotor.run(FORWARD);
	}
	else
	{
		rmotor.run(FORWARD);
		lmotor.run(BACKWARD);
	}
	sleep(t);
	rmotor.run(FORWARD);
	lmotor.run(FORWARD);
}

