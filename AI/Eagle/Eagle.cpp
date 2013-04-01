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
RobotState Eagle::IdentifyTarget(RobotState &ourRobotState, RobotState &enemyRobotState, Vector2 ballPos)
{
	RobotState targetState;

	Vector2 ourRobotPos = ourRobotState.Position();
	Vector2 enemyRobotPos = enemyRobotState.Position();

    Vector2 ourGoalCentre = GoalCentrePosition(m_pitchSide);
    Vector2 enemyGoalCentre;

    if (m_pitchSide == eLeftSide)
    {
        enemyGoalCentre = GoalCentrePosition(eRightSide);
    }
    else
    {
        enemyGoalCentre = GoalCentrePosition(eLeftSide);
    }
    
    ballPos.Clamp(Vector2(0,0), Vector2(m_pitchSizeX-1, m_pitchSizeY-1));

    ourRobotState.SetHasBall(DoesRobotHaveBall(ourRobotState, ballPos));
    enemyRobotState.SetHasBall(DoesRobotHaveBall(enemyRobotState, ballPos));

    if (m_state == ePenaltyAttack)
	{
         // When taking a penalty, we want to find a free position to kick to.
        // Position should stay the same - we only want to re-orientate.
        targetState.SetPosition(ourRobotPos);

        // We'll do this by checking for intersections between us and three positions on the goal line.
        Vector2 targetPositions[3];

        targetPositions[0] = enemyGoalCentre;
        targetPositions[1] = enemyGoalCentre - Vector2(0,50);
        targetPositions[2] = enemyGoalCentre + Vector2(0,50);

        int arrayLength = sizeof(targetPositions)/sizeof(Vector2);

        Vector2 optimalShootingTarget;
        float bestDistanceFromEnemy = 0;

        // Iterate through the positions, finding the best one, based on if it's unblocked and how far it is from the enemy robot.
        for (int i=0; i < arrayLength; i++)
        {
            // Check if the target is unblocked.
            bool isBlocked = m_intersection.LineCircleIntersection(ourRobotPos, targetPositions[i], enemyRobotPos, ROBOT_RADIUS);

            if (isBlocked)
            {
                continue;
            }

            float distanceSqdToEnemy = enemyRobotPos.DistanceSquared(&targetPositions[i]);

            // Check if this beats our previous best distance.
            if (distanceSqdToEnemy > bestDistanceFromEnemy)
            {
                bestDistanceFromEnemy = distanceSqdToEnemy;
                optimalShootingTarget = targetPositions[i];
            }

            // Check that we do actually have a target set.
            if (optimalShootingTarget.IsSet())
            {
                float angleToTarget = ourRobotPos.GetAngleTo(&optimalShootingTarget);

                targetState.SetOrientation(angleToTarget);
            }
            else
            {
                targetState.SetOrientation(ourRobotState.Orientation());
            }
        }


	}
    else if (m_state == ePenaltyDefend)
	{
		// When defending, we're permitted to move up and down the goalline.
		// Orientation should stay the same.
		targetState.SetOrientation(ourRobotState.Orientation());

		// X-axis position should be the same, y-axis should be a position extrapolated in the direction of the enemy robot.
		//float extrapolationGradient = tan(enemyRobotState.Orientation());
		// I'm experimenting with extrapolating on a line between the robot and ball instead.
		float extrapolationGradient = enemyRobotPos.Gradient(&ballPos);

		int extrapolatedY = enemyRobotPos.Y() + ((ourRobotPos.X() - enemyRobotPos.X()) * extrapolationGradient); 

		Vector2 proposedPosition(ourRobotPos.X(), extrapolatedY);
		proposedPosition.Clamp(Vector2(0,0), Vector2(m_pitchSizeX-1, m_pitchSizeY-1));

		targetState.SetPosition(proposedPosition);
	}
	else
	{
		// If we're here, assume we're in open play.
		if (!ourRobotState.HasBall())
		{
			// Check if the enemy robot has the ball.
			if (enemyRobotState.HasBall())
            {
				if (m_pitchSide == eLeftSide)
				{
                    targetState.SetPosition(ourGoalCentre.X() + 50, ourGoalCentre.Y());
					targetState.SetOrientation(0);
				}
				else
				{
                    targetState.SetPosition(ourGoalCentre.X() - 50, ourGoalCentre.Y());
					targetState.SetOrientation(M_PI);
				}
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
			if (proposedPosition.Distance(&enemyRobotPos) < 2*ROBOT_RADIUS)
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

			proposedPosition.Clamp(Vector2(0,0), Vector2(m_pitchSizeX-1, m_pitchSizeY-1));
			targetState.SetPosition(proposedPosition);
            targetState.SetOrientation(proposedPosition.GetAngleTo(&enemyGoalCentre));
		}

		targetState.SetPosition((int)targetState.Position().X(), (int)targetState.Position().Y());
	}

	
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
        distanceThresh = 70.0f;
    }
    else
    {
        angleThresh = M_PI_4;
        distanceThresh = 45.0f;
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

Vector2 Eagle::GoalCentrePosition(TPitchSide pitchSide)
{
	// If we're the left side, then the goal we're aiming for is at the right side of the pitch
	// Else, we're aiming for the goal on the far left
	int goalX;

    if(pitchSide == eLeftSide)
	{
        goalX = 0;
	}
	else
	{
        goalX = m_pitchSizeX;
	}
	
	int goalY = m_pitchSizeY / 2;

	Vector2 goalPosition = Vector2(goalX, goalY);

	return goalPosition;
}


bool Eagle::ShouldWeShoot(RobotState ourRobotState, RobotState enemyRobotState, Vector2 ballPos)
{
// In simplistic terms, we should shoot if we're close enough to the goal,
	// we've got the ball, and there is clear line-of-sight to goal
    Vector2 goalPosition;

    if (m_pitchSide == eLeftSide)
    {
        goalPosition = GoalCentrePosition(eRightSide);
    }
    else
    {
        goalPosition = GoalCentrePosition(eLeftSide);
    }

    Vector2 ourRobotPos = ourRobotState.Position();

    const float angleThresh = M_PI_4;

	float distToGoal = ourRobotState.Position().Distance(&goalPosition);

	// Ian reckons this should be 1/4 of the pitch, but I'm living dangerously and have gone for 1/3.
	float distanceThreshold = KICKING_THRESHOLD * m_pitchSizeX;

	// Commenting this out now - we need to account for the ball being invisible.
	//bool weHaveBall = DoWeHaveBall(ourRobotState, ballPos);
	bool closeToGoal = distToGoal < distanceThreshold;
	
	// Check that the enemy robot isn't obstructing the ball.
    bool isGoalClear = false;

    float angleToGoal = fmod(ourRobotPos.GetAngleTo(&goalPosition), 2*M_PI);
    float ourOrientation = fmod(ourRobotState.Orientation(), 2*M_PI);

    float angleDifference = fmod(fabs(angleToGoal - ourOrientation), 2*M_PI);
   //bool isFacingGoal = (angleDifference < angleThresh) || (angleDifference > (2*M_PI) - angleThresh);

	// Check if we're facing somewhere into the goal.
	// Extrapolate to the goal-line position we're facing.
    float extrapolatedYCoord = ourRobotPos.Y() + (ourRobotPos.Gradient(&ballPos) * (goalPosition.X() - ourRobotPos.X()));

	// The goal is in the middle of the pitch, constituting half its length.
	float goalTop = 0.75 * m_pitchSizeY;
	float goalBottom = 0.25 * m_pitchSizeY;

	bool isFacingGoal = (extrapolatedYCoord >= goalBottom) && (extrapolatedYCoord <= goalTop);
	
	// The position we're aiming for.
	Vector2 goalTarget(m_pitchSizeX, extrapolatedYCoord);

    if (!m_intersection.LineCircleIntersection(ourRobotPos, goalTarget, enemyRobotState.Position(), ROBOT_RADIUS))
	{
		isGoalClear = true;
	}

    if (closeToGoal && isGoalClear && isFacingGoal)
	{
		return true;
	}
	
	return false;
}
