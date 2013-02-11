/*
 * Movement.h
 *
 *  Created on: Jan 25, 2013
 *      Author: Georgi
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_
#include "../AI/MathTypes/Vector2.h"
#include "Motor.h"
#include "Compute.h"


class Movement
{
public:
    
	void Initialise();
	void RunMovement();    	
	void IssueSpeed(Vector2 targetLocation, Vector2 ourRobotLocation);
	
	int GetMotorSpeed(int motorNumber);
private:
	Compute comp;	
	Motor motor1;
	Motor motor2;
	Motor motor3;
	Motor motor4;
};



#endif /* MOVEMENT_H_ */
