#include "Compute.h"
#include <math.h>
#include <vector>

Compute::Compute()
{

}

float Compute::ComputeDistance(Vector2 targetLocation, Vector2 ourRobotLocation)
{
	return sqrt((targetLocation.X() - ourRobotLocation.X())*(targetLocation.X() - ourRobotLocation.X()) + (targetLocation.Y() - ourRobotLocation.Y())*(targetLocation.Y() - ourRobotLocation.Y()));
}

float Compute::ComputeAngle(Vector2 targetLocation, Vector2 ourRobotLocation)
{
	return 0;
}
