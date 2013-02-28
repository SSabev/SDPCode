#include "Eagle.h"

#include <cmath>

Eagle::Eagle()
{
	
}

/*! 
 * Sets the system state, as defined in shared memory. Eagle uses this to determine its behaviour.
*/
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


bool Eagle::DoWeHaveBall(RobotState ourRobotState, Vector2 ballPos) {
	Vector2 robotPos = ourRobotState.Position();

	float angleThresh = M_PI_4;
	float distanceThresh = 75.0;
	
	float angleToBall = fmod(robotPos.GetAngleTo(&ballPos), (2*M_PI));
	float robotOrientation = fmod(ourRobotState.Orientation(), (2*M_PI));

	float distanceToBall = robotPos.Distance(&ballPos);
	
	bool withinOrientationThresh = fabs(robotOrientation - angleToBall) < angleThresh;
	bool withinProximityThresh = distanceToBall < distanceThresh;
	
	if(withinOrientationThresh && withinProximityThresh) {
		return true;
	} 
	
	return false;
}
