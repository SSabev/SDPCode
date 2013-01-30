/*
 * movement.cpp
 *
 *  Created on: Jan 25, 2013
 *      Author: Georgi
 */
#include "Movement.h"
#include "Motor.h"
#include "Compute.h"
//#include "MathTypes/Vector2.h"
//#include <vector>

Movement::Movement()
{
Motor motor1 = Motor();
Motor motor2 = Motor();
Motor motor3 = Motor();
Motor motor4 = Motor();
}

void Movement::IssueSpeed(Vector2 targetLocation, Vector2 ourRobotLocation)
{
Compute comp = Compute();
int angleToTurn = comp.ComputeOrientationAngle(targetLocation, ourRobotLocation);
comp.ComputeMotorSpeeds(angleToTurn);

motor1.SetSpeed(comp.GetMotorSpeed(0));
motor2.SetSpeed(comp.GetMotorSpeed(1));
motor3.SetSpeed(comp.GetMotorSpeed(2));
motor4.SetSpeed(comp.GetMotorSpeed(3));

}

int Movement::GetMotorSpeed(int motorNumber)
{
    if(motorNumber == 1) return motor1.GetSpeed();
	if(motorNumber == 2) return motor2.GetSpeed();
	if(motorNumber == 3) return motor3.GetSpeed();
	if(motorNumber == 4) return motor4.GetSpeed();
	return 0;
	
}



