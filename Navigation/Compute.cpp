#include "Compute.h"
#define _USE_MATH_DEFINES
#include "../AI/MathTypes/Vector2.h"
#include <math.h>
#include <vector>

Compute::Compute() {
	
}

/*
 * 	Computes the distance to be travelled by the robot in order to get to the new location
 */

float Compute::ComputeDistance(Vector2 targetLocation, Vector2 ourRobotLocation) {
	return sqrt((targetLocation.X() - ourRobotLocation.X())
			* (targetLocation.X() - ourRobotLocation.X()) + (targetLocation.Y()
			- ourRobotLocation.Y()) * (targetLocation.Y()
			- ourRobotLocation.Y()));
}

/*
 * Obsolete code to be determined if needed


 float Compute::ComputeAngleToTurnBy(Vector2 targetLocation, Vector2 targetLocation, int orientation)
 {
 return 0;
 }
 *
 */

/*
 * 	Computes the angle between the current orientation and the orientation after the move
 *	by which we will need to turn
 *	Will need further work if we dont use the holonomic wheels
 *	TODO
 */
float Compute::ComputeOrientationAngle(Vector2 targetLocation,
		Vector2 ourRobotLocation) {
	Vector2 normalisedLocation = ourRobotLocation - targetLocation;
	float newAngle = round(FromRadiansToDegrees(atan2(normalisedLocation.Y(), normalisedLocation.X())));
	return newAngle;
}


/*
 * 	Transforms the angle from radians to degrees
 */

float Compute::FromRadiansToDegrees(float radians) {
	return radians * 180 / M_PI;
}

/*
 *
 */
int Compute::GetMotorSpeed(int motorNumber)
{
    return motorSpeed[motorNumber];
}

/*
 *
 */
float Compute::round(double d)
{
  return floor(d + 0.5);
}


/*
 * Experimental function to compute the speeds of four motors that
 * are to be used in conjunction with the omniwheels.
 * The function computes the percentage of the speed for each wheel
 * so it needs to be scaled to whatever out of 900 we are comfortable with
 * If we decide to drop the omniwheels this code becomes obsolete.
 *
 */

void Compute::ComputeMotorSpeeds(int angle) {
	int speed1, speed2;
	if (angle <= 90) {
		if (angle > (90 - angle)) {
			speed1 = 100;
			speed2 = ((90 - angle) * 100) / angle;
		} else {
			speed2 = 100;
			speed1 = ((angle) * 100) / (90 - angle);
		}
		motorSpeed[0] = speed2;
		motorSpeed[1] = speed1;
        motorSpeed[2] = (speed2);
        motorSpeed[3] = (speed1);
	} else if (angle > 90 && angle <= 180) {
		angle = angle - 90;
		if (angle > (90 - angle)) {
			speed1 = 100;
			speed2 = ((90 - angle) * 100) / angle;
		} else {
			speed2 = 100;
			speed1 = ((angle) * 100) / (90 - angle);
		}
        motorSpeed[0] = (speed1);
		motorSpeed[1] = (speed2);
		motorSpeed[2] = speed1;
        motorSpeed[3] = (speed2);
	} else if (angle > 180 && angle <= 270) {
		angle = angle - 180;
		if (angle > (90 - angle)) {
			speed1 = 100;
			speed2 = ((90 - angle) * 100) / angle;
		} else {
			speed2 = 100;
			speed1 = ((angle) * 100) / (90 - angle);
		}
        motorSpeed[0] =  speed2;
        motorSpeed[1] = (speed1);
		motorSpeed[2] = speed2;
		motorSpeed[3] = speed1;
	} else {
		angle = angle - 270;
		if (angle > (90 - angle)) {
			speed1 = 100;
			speed2 = ((90 - angle) * 100) / angle;
		} else {
			speed2 = 100;
			speed1 = ((angle) * 100) / (90 - angle);
		}
		motorSpeed[0] =  speed1;
        motorSpeed[1] = (speed2);
        motorSpeed[2] = ((speed1));
		motorSpeed[3] = speed2;
	}
}

