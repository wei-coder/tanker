
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
	void left();
	void right();
	void backward();
	void forward();
	void stop();
	void moveto(int x,int y);
};

