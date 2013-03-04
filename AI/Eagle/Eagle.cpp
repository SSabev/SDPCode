#include "Eagle.h"

#include <cmath>

#define KICKING_THRESHOLD 0.33
#define ROBOT_RADIUS 35

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
	ballPos.Clamp(Vector2(0,0), Vector2(m_pitchSizeX-1, m_pitchSizeY-1));

	if (!DoWeHaveBall(ourRobotState, ballPos))
	{
		// TODO - If we're not behind the ball, make this a position behind the ball.
		if ((m_pitchSide == eLeftSide) && (ourRobotState.Position().X() > ballPos.X() - 40))
		{
			targetState.SetPosition(ballPos - Vector2(40,0));
			targetState.SetOrientation(0);
		}
		else if ((m_pitchSide == eRightSide) && (ourRobotState.Position().X() < ballPos.X() + 40))
		{
			targetState.SetPosition(ballPos + Vector2(40,0));
			targetState.SetOrientation(M_PI);
		}
		else
		{
			// If we don't have the ball, the aim should be to move to the ball.
			targetState.SetPosition(ballPos);
			
			if (m_pitchSide == eLeftSide)
			{
				targetState.SetOrientation(0);
			}
			else
			{
				targetState.SetOrientation(M_PI);
			}
		}
	}
	else
	{
		// If we have the ball, let's move to a more appropriate place.
		// This is done regardless of whether we're shooting or not.
		/*
			This should depend on:
			1. The opposite half of the pitch to the enemy robot.
			2. Within the kicking threshold.
			3. Fairly central, but outside of the enemy robot's radius
			4. Orientated towards the goal
		*/
		bool isEnemyOnBottomSide;
	
		// Determine which half of the pitch the enemy robot is on.
		if (enemyRobotState.Position().Y() < m_pitchSizeY/2)
		{
			isEnemyOnBottomSide = true;
		}
		else
		{
			isEnemyOnBottomSide = false;
		}

		// Determine where the kicking threshold is for this side of the pitch.
		float kickingPosition;

		if (m_pitchSide == eLeftSide)
		{
			kickingPosition = m_pitchSizeX - ((KICKING_THRESHOLD/2)*m_pitchSizeX);
		}
		else
		{
			kickingPosition = (KICKING_THRESHOLD/2)*m_pitchSizeX;
		}

		// Check if the position is within two robot radii of the enemy.
		Vector2 proposedPosition = Vector2(kickingPosition,m_pitchSizeY/2);
		Vector2 enemyRobotPosition = enemyRobotState.Position();

		// Check if the proposed position is too close to the enemy robot to be used.
		if (proposedPosition.Distance(&enemyRobotPosition) < 2*ROBOT_RADIUS)
		{
			float adjustedXPosition;

			if (isEnemyOnBottomSide)
			{
				adjustedXPosition = kickingPosition + 2*ROBOT_RADIUS;
			}
			else
			{
				adjustedXPosition = kickingPosition - 2*ROBOT_RADIUS;
			}

			proposedPosition = Vector2(adjustedXPosition, proposedPosition.Y());
		}

		Vector2 goalCentre = GoalCentrePosition();

		proposedPosition.Clamp(Vector2(0,0), Vector2(m_pitchSizeX-1, m_pitchSizeY-1));
		targetState.SetPosition(proposedPosition);
		targetState.SetOrientation(proposedPosition.GetAngleTo(&goalCentre));
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
	float distanceThreshold = KICKING_THRESHOLD * m_pitchSizeX;

	bool weHaveBall = DoWeHaveBall(ourRobotState, ballPos);
	bool closeToGoal = distToGoal < distanceThreshold;
	

	// Check that the enemy robot isn't obstructing the ball.
	bool isGoalClear = false;

	if (!m_intersection.LineCircleIntersection(ourRobotState.Position(), enemyRobotState.Position(), goalPosition, ROBOT_RADIUS))
	{
		isGoalClear = true;
	}

	if (weHaveBall && closeToGoal && isGoalClear)
	{
		return true;
	}
	
	return false;	
}