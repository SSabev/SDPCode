#include "Eagle.h"

#include <cmath>

#define KICKING_THRESHOLD 0.6
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

	Vector2 enemyRobotPosition = enemyRobotState.Position();

	ourRobotState.SetHasBall(DoesRobotHaveBall(ourRobotState, ballPos));
	enemyRobotState.SetHasBall(DoesRobotHaveBall(enemyRobotState, ballPos));

	if (!ourRobotState.HasBall())
	{
		// Check if the ball is within the enemy robot's radius or if it's against our back wall.
		if ((enemyRobotPosition.Distance(&ballPos) < 2*ROBOT_RADIUS) || ((m_pitchSide == eLeftSide) && (ballPos.X() < 50)) || ((m_pitchSide == eRightSide) && (ballPos.X() > m_pitchSizeX - 50)))
		{
			/*// TODO: In this case, we want to assume a defensive position at our goal mouth.
			Vector2 proposedDefensivePosition;

			if (m_pitchSide == eLeftSide)
			{
				proposedDefensivePosition = Vector2(40, m_pitchSizeY/2);
			}
			else
			{
				proposedDefensivePosition = Vector2(m_pitchSizeX - 40, m_pitchSizeY/2);
			}

			// TODO: Check if the enemy robot is in the way of the position we want.

			targetState.SetPosition(proposedDefensivePosition);
			targetState.SetOrientation(proposedDefensivePosition.GetAngleTo(&ballPos));*/

			// Temporary - robot will stop still in this instance.
			targetState = ourRobotState;
		}
		else if ((m_pitchSide == eLeftSide) && (ourRobotState.Position().X() > ballPos.X() - 30))
		{
			targetState.SetPosition(ballPos - Vector2(40,0));
			targetState.SetOrientation(0);
		}
		else if ((m_pitchSide == eRightSide) && (ourRobotState.Position().X() < ballPos.X() + 30))
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

		// Check if the proposed position is too close to the enemy robot to be used.
		if (proposedPosition.Distance(&enemyRobotPosition) < 2*ROBOT_RADIUS)
		{
			float adjustedYPosition;

			if (isEnemyOnBottomSide)
			{
				adjustedYPosition = proposedPosition.Y() + 2*ROBOT_RADIUS;
			}
			else
			{
				adjustedYPosition = proposedPosition.Y() - 2*ROBOT_RADIUS;
			}

			proposedPosition = Vector2( proposedPosition.X(), adjustedYPosition);
		}

		Vector2 goalCentre = GoalCentrePosition();

		proposedPosition.Clamp(Vector2(0,0), Vector2(m_pitchSizeX-1, m_pitchSizeY-1));
		targetState.SetPosition(proposedPosition);
		targetState.SetOrientation(proposedPosition.GetAngleTo(&goalCentre));
	}

	targetState.SetPosition((int)targetState.Position().X(), (int)targetState.Position().Y());

	return targetState;
}

bool Eagle::DoesRobotHaveBall(RobotState robotState, Vector2 ballPos) 
{
	Vector2 robotPos = robotState.Position();

    float angleThresh;
	float distanceThresh;

	if (robotState.HasBall())
	{
		angleThresh = M_PI_2;
		distanceThresh = 90.0f;
	}
	else
	{
		angleThresh = M_PI_4;
		distanceThresh = 55.0f;
	}
	
	float angleToBall = fmod(robotPos.GetAngleTo(&ballPos), (2*M_PI));
	float robotOrientation = fmod(robotState.Orientation(), (2*M_PI));

	float distanceToBall = robotPos.Distance(&ballPos);
	
    float orientationDiff = fmod(fabs(robotOrientation - angleToBall), (2*M_PI));
    bool withinOrientationThresh = (orientationDiff < angleThresh) || (2*M_PI - orientationDiff < angleThresh);
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

    const float angleThresh = M_PI_4;

	float distToGoal = ourRobotState.Position().Distance(&goalPosition);

	// Ian reckons this should be 1/4 of the pitch, but I'm living dangerously and have gone for 1/3.
	float distanceThreshold = KICKING_THRESHOLD * m_pitchSizeX;

	// Commenting this out now - we need to account for the ball being invisible.
	//bool weHaveBall = DoWeHaveBall(ourRobotState, ballPos);
	bool closeToGoal = distToGoal < distanceThreshold;
	
	// Check that the enemy robot isn't obstructing the ball.
    bool isGoalClear = false;

    float angleToGoal = fmod(ourRobotState.Position().GetAngleTo(&goalPosition), 2*M_PI);
    float ourOrientation = fmod(ourRobotState.Orientation(), 2*M_PI);

    float angleDifference = fmod(fabs(angleToGoal - ourOrientation), 2*M_PI);
   //bool isFacingGoal = (angleDifference < angleThresh) || (angleDifference > (2*M_PI) - angleThresh);

	// Check if we're facing somewhere into the goal.
	// Extrapolate to the goal-line position we're facing.
	float extrapolatedYCoord = ourRobotState.Position().Y() + (ourRobotState.Position().Gradient(&ballPos) * (m_pitchSizeX - ourRobotState.Position().X()));

	// The goal is in the middle of the pitch, constituting half its length.
	float goalTop = 0.75 * m_pitchSizeY;
	float goalBottom = 0.25 * m_pitchSizeY;

	bool isFacingGoal = (extrapolatedYCoord >= goalBottom) && (extrapolatedYCoord <= goalTop);
	
	// The position we're aiming for.
	Vector2 goalTarget(m_pitchSizeX, extrapolatedYCoord);

	if (!m_intersection.LineCircleIntersection(ourRobotState.Position(), goalTarget, enemyRobotState.Position(), ROBOT_RADIUS))
	{
		isGoalClear = true;
	}

    if (closeToGoal && isGoalClear && isFacingGoal)
	{
		return true;
	}
	
	return false;	
}
