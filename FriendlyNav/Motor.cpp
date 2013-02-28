#include "Motor.h"
#include <math.h>
#include <../Shared/SharedMem.h>

Motor::Motor()
{
	SetSpeed(0);
	SetTime(0);
}

Motor::Motor(float x, float y)
{
	SetSpeed(x);
	SetTime(y);
}

void Motor::SetSpeed(int x)
{
	speed = x;
}

void Motor::SetTime(int x)
{
	time = x;
}

void Motor::SetWheelRotation(float x)
{
	wheelRotation = x;
}

int Motor::GetSpeed()
{
	return speed;
}

int Motor::GetTime()
{
	return time;
}

float Motor::GetWheelRotation()
{
	return wheelRotation;
}
