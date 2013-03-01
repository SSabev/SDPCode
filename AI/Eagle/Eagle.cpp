#include "Eagle.h"

#include <cmath>

Eagle::Eagle()
{
	m_intersection = Intersection();
}

/*!
* Sets the game data stored in the shared memory.
*/
void Eagle::SetSharedData(TSystemState state, int pitchSizeX, int pitchSizeY, TPitchSide pitchSide)
{
	m_state = state;
	m_pitchSide = pitchSide;

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

	if (!DoWeHaveBall(ourRobotState, ballPos))
	{
		// If we don't have the ball, the aim should be to move to the ball.
		ballPos.Clamp(Vector2(0,0), Vector2(m_pitchSizeX-1, m_pitchSizeY-1));
		targetState.SetPosition(ballPos);
		targetState.SetOrientation(0);
	}
	else
	{
		// If we have the ball, but shouldn't shoot, let's move to a more appropriate place.
		// This is done regardless of whether we're shooting or not.
		// TODO
	}

	return targetState;
}


bool Eagle::DoWeHaveBall(RobotState ourRobotState, Vector2 ballPos) 
{
	Vector2 robotPos = ourRobotState.Position();

	float angleThresh = M_PI_4;
	float distanceThresh = 75.0f;
	
	float angleToBall = fmod(robotPos.GetAngleTo(&ballPos), (2*M_PI));
	float robotOrientation = fmod(ourRobotState.Orientation(), (2*M_PI));

	float distanceToBall = robotPos.Distance(&ballPos);
	
	bool withinOrientationThresh = fabs(robotOrientation - angleToBall) < angleThresh;
	bool withinProximityThresh = distanceToBall < distanceThresh;
	
	if(withinOrientationThresh && withinProximityThresh)
	{
		return true;
	} 
	
	return false;
}


Vector2 Eagle::GoalCentrePosition()
{
	// If we're the left side, then the goal we're aiming for is at the right side of the pitch
	// Else, we're aiming for the goal on the far left
	int goalX;
	if(m_pitchSide == eLeftSide)
	{
		goalX = m_pitchSizeX;
	}
	else
	{
		goalX = 0;
	}
	
	int goalY = m_pitchSizeY / 2;

	Vector2 goalPosition = Vector2(goalX, goalY);

	return goalPosition;
}


bool Eagle::ShouldWeShoot(RobotState ourRobotState, RobotState enemyRobotState, Vector2 ballPos)
{
	// In simplistic terms, we should shoot if we're close enough to the goal,
	// we've got the ball, and there is clear line-of-sight to goal
	Vector2 goalPosition = GoalCentrePosition();

	float distToGoal = ourRobotState.Position().Distance(&goalPosition);

	// Ian reckons this should be 1/4 of the pitch, but I'm living dangerously and have gone for 1/3.
	float distanceThreshold = 0.33f * m_pitchSizeX;

	bool weHaveBall = DoWeHaveBall(ourRobotState, ballPos);
	bool closeToGoal = distToGoal < distanceThreshold;

	// We're going to put a circle around the enemy robot to make sure we don't collide with it.
	float robotRadius = 40;

	// Check that the enemy robot isn't obstructing the ball.
	bool isGoalClear = false;

	if (!m_intersection.LineCircleIntersection(ourRobotState.Position(), enemyRobotState.Position(), goalPosition, robotRadius))
	{
		isGoalClear = true;
	}

	if (weHaveBall && closeToGoal && isGoalClear)
	{
		return true;
	}
	
	return false;	
}