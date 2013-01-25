/*
 * Compute.h
 *
 *  Created on: Jan 25, 2013
 *      Author: Georgi
 */

#ifndef COMPUTE_H_
#define COMPUTE_H_
#include "MathTypes/Vector2.h"
#include <vector>

class Compute
{
public:
	Compute();

	float ComputeDistance(Vector2 targetLocation, Vector2 ourRobotLocation);

	float ComputeAngle(Vector2 targetLocation, Vector2 ourRobotLocation);
};

#endif
