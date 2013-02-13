#include "Eagle.h"

#include <cmath>

Eagle::Eagle()
{
	
}

void Eagle::SetState(TSystemState state)
{
	m_state = state;
}

RobotState Eagle::IdentifyTarget(RobotState ourRobotState, RobotState enemyRobotState, Vector2 ballPos)
{
	// For now, this is just the ball position.
	RobotState targetState;

	if (m_state == eDribbleBall)
	{
		// Try to work out if we're in position to start dribbling.
		if (fabs(ourRobotState.Position().Y() - ballPos.Y()) < 40)
		{
			targetState.SetPosition(ballPos + Vector2(25,0));
		}
		else
		{
			targetState.SetPosition(ballPos - Vector2(100,0));
		}
	}
	else
	{
		// Else we're navving to the ball.
		targetState.SetPosition(ballPos);
	}

	targetState.SetOrientation(0);

	return targetState;
}