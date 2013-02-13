#include "Eagle.h"

#include <cmath>

Eagle::Eagle()
{
	
}

void Eagle::SharedMemPointer(TShMem* pSharedMem)
{
	m_pSharedMem = pSharedMem;
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
		if (fabs(ourRobotState.Position().Y() - ballPos.Y()) < 20)
		{
			// Hack to indicate that the AI's happy with where we've arrived.
			m_pSharedMem->systemState = eLeftPenalty;
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