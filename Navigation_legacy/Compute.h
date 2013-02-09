/*
 * Compute.h
 *
 *  Created on: Jan 25, 2013
 *      Author: Georgi
 */

#ifndef COMPUTE_H_
#define COMPUTE_H_
#include "../AI/MathTypes/Vector2.h"
#include <vector>

class Compute
{
public:
	Compute();

	float ComputeDistance(Vector2 targetLocation, Vector2 ourRobotLocation);

	float ComputeAngle(Vector2 targetLocation, Vector2 ourRobotLocation);

	float ComputeOrientationAngle(Vector2 targetLocation, Vector2 ourRobotLocation);
	
    float ComputeAngleToTurnBy(Vector2 targetLocation, Vector2 ourRobotLocation, float orientation);
	
	void ComputeMotorSpeeds(int angle);
	
	int GetMotorSpeed(int motorNumber);
	
    float round(double d);

	float FromRadiansToDegrees(float radians);

private:

	int wheelRadius;
	int motorSpeed[4];

};

#endif
