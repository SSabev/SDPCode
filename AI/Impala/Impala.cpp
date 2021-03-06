#include "Impala.h"

#include <cmath>

Impala::Impala()
{
	
}

/*!
 * This is the Impala's only public method and should be called in order to run both the path-smoothing and 
 * path-reduction. Its purpose is to take a raw A* path, smooth the hard corners and remove redundant points. 
 * It also recalculates the desired robot orientation for each point. 
 *
 * The 'numAdditionalPasses' argument is optional and will default to 0 if not supplied. Note that one run of 
 * the path smoothing will always be executed, in addition to the number specified in the parameter. This 
 * parameter can be increased to provide a smoother path, but at the cost of processing time.
*/
std::list<RobotState> Impala::SmoothPath(std::list<RobotState> aStarPath, bool doWeHaveBall, int numAdditionalPasses)
{
	std::list<Vector2> aStarPositions;
	std::list<RobotState>::iterator it;

	for (it = aStarPath.begin(); it != aStarPath.end(); it++)
	{
		aStarPositions.push_back(it->Position());
	}

	std::list<Vector2> smoothedPath = RunPass(aStarPositions);

	for (int i=0; i<numAdditionalPasses; i++)
	{
		smoothedPath = RunPass(smoothedPath);
	}

	std::vector<Vector2> collapsedPositions = CollapsePoints(smoothedPath);

	// Now we want to add the orientations back in.
	std::list<RobotState> collapsedPath;

	std::list<Vector2>::iterator positionIt;


    for (int i=0; i<collapsedPositions.size(); i++)
	{
		float angleToNextPoint;
        Vector2 goalCentrePos = GoalCentrePosition();

		// If this is any point other than the first point, we want to be facing the goal.
		if (i==0)
		{
			angleToNextPoint = aStarPath.front().Orientation();
		}
		else
		{
			if (!doWeHaveBall)
			{
				if (i == collapsedPositions.size()-1)
				{
				 	if ((m_state == eMatch) || (m_state == eIDLE))
		                    	{
		                   	     angleToNextPoint = collapsedPositions[i-1].GetAngleTo(&aStarPositions.back());
		                    	}
		                    	else
		                    	{
		                        	// We want to preserve the target orientation.
		                        	angleToNextPoint = aStarPath.back().Orientation();
		                    	}
				}
				else
				{
					angleToNextPoint = collapsedPositions[i].GetAngleTo(&aStarPositions.back());
				}
			}
			else
			{
				angleToNextPoint = collapsedPositions[i].GetAngleTo(&goalCentrePos);
			}
		}

		collapsedPath.push_back(RobotState(collapsedPositions[i], angleToNextPoint));
	}

	return collapsedPath;
}

/*!
 * Runs a pass of the path-smoothing algorithm. It works by taking each set of 3 consecutive points and 
 * constructing a triangle from them. The triangle centroid (the point found by at the meeting point of the 
 * three lines which run from each point to the centre of the opposite edge) then replaces the middle of the 
 * 3 points in our path. This acts to make changes in direction more gradual.
 * 
 * Note that whilst more runs of this algorithm will lead to a smoother path, it will ultimately tend towards 
 * a straight line between the start point and destination.
*/
std::list<Vector2> Impala::RunPass(std::list<Vector2> path)
{
	std::vector<Vector2> pathVector(path.begin(), path.end());

	// This method attempts to find the centroid of a triangle, where the three points 
	// are a given one and those immediately preceding and following it in the path.
	std::list<Vector2> smoothedPath;

	smoothedPath.push_front(pathVector.front());

	// Note  1 <= i < size-1, as we want a point either side of it.
	for (int i=1; i<pathVector.size()-1; i++)
	{
		Vector2 midpoint = (pathVector[i-1] + pathVector[i] + pathVector[i+1])/3;
	
		smoothedPath.push_back(midpoint);
	}

	smoothedPath.push_back(pathVector.back());

	return smoothedPath;
}



void Impala::SetSharedData(TSystemState state, int pitchSizeX, int pitchSizeY, TPitchSide pitchSide)
{
	m_state = state;
	m_pitchSide = pitchSide;

	m_pitchSizeX = pitchSizeX;
	m_pitchSizeY = pitchSizeY;
}

Vector2 Impala::GoalCentrePosition()
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

/*!
* Reduces the number of waypoints by removing those which are on a straight line (and not the endpoints), making
* them redundant. This is done by comparing the initial pair of points and finding the gradient between them (the 
* 'original gradient'). If the next pair of points has a gradient which within a given threshold of the original 
* gradient, then this is determined to be part of a straight line. The point between the two endpoints can be 
* removed.
*
* Otherwise, if the gradient difference is too high, this is not a straight line, no points can be collapsed and 
* the second gradient becomes the new 'original gradient'.
*/


std::vector<Vector2> Impala::CollapsePoints(std::list<Vector2> path)
{
	const float GRADIENT_THRESHOLD = 0.1;

	std::vector<Vector2> pathVector(path.begin(), path.end());

	std::vector<Vector2> collapsedPath;

	collapsedPath.push_back(pathVector[0]);

	// Size-3 as we need at least three points to collapse one of them.
	// And the last one will be added anyway.
	for (int i=0; i<pathVector.size()-3; i++)
	{
		float originalGradient = pathVector[i].Gradient(&pathVector[i+1]);

		for (int j=i+1; j < pathVector.size()-2; j++)
		{
			float currentGradient = pathVector[j].Gradient(&pathVector[j+1]);
			float gradientDifference = std::abs(currentGradient - originalGradient);

			if (gradientDifference > GRADIENT_THRESHOLD)
			{
				// We can't consume this point, the gradient difference is too large.
				// Add the last successful point to our list and change i so we can start a new loop.
				collapsedPath.push_back(pathVector[j]);
				i=j;
				break;
			}
		}
	}

	collapsedPath.push_back(pathVector[pathVector.size()-1]);
	
	return collapsedPath;
}
