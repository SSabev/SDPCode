#include "Eagle.h"

#include <cmath>

Eagle::Eagle()
{
	
}

RobotState Eagle::IdentifyTarget(RobotState ourRobotState, RobotState enemyRobotState, Vector2 ballPos)
{
	// For now, this is just the ball position.
	RobotState targetState;

	// Try to work out if we're in position to start dribbling.
	if (fabs(ourRobotState.Position().Y() - ballPos.Y()) < 10)
	{
		targetState.SetPosition(ballPos + Vector2(25,0));
	}
	else
	{
		targetState.SetPosition(ballPos - Vector2(25,0));
	}
	
	targetState.SetOrientation(0);

	return targetState;
}