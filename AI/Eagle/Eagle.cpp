#include "Eagle.h"

#include <cmath>

Eagle::Eagle()
{
	
}

void Eagle::SetState(TSystemState state)
{
	m_state = state;
}

/*!
* Sets the dimensions of the pitch. This will be defined in the shared memory, in terms of pixels as seen by the vision module.
*/
void Eagle::SetPitchDimensions(int pitchSizeX, int pitchSizeY)
{
	m_pitchSizeX = pitchSizeX;
	m_pitchSizeY = pitchSizeY;
}

/*!
* Identify the target state that we wish the robot to be in. This will be the target which the A* algorithm plots towards.
*/
RobotState Eagle::IdentifyTarget(RobotState ourRobotState, RobotState enemyRobotState, Vector2 ballPos)
{
	// For now, this is just the ball position.
	RobotState targetState;

	ballPos.Clamp(Vector2(0,0), Vector2(m_pitchSizeX-1, m_pitchSizeY-1));

	targetState.SetPosition(ballPos);

	targetState.SetOrientation(0);

	return targetState;
}
