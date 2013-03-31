#include "AStar.h"

#include <float.h>
#include <assert.h>
#include <iostream>
#include <fstream>

#include <vector>
#include <sys/time.h>
#include <cmath>

// I'm experimenting with penalising the heuristic, otherwise it results in expanding 
// too many nodes needlessly.
// This will make it inadmissable (not really A* anymore), but given that we know there are 
// few obstacles on the pitch and a truly optimal path isn't really necessary, we can 
// probably get away with it. **The speed benefit is insane - factor of 100s**
const float HEURISTIC_PENALTY = 2.5f;

const double TIMER_EXPIRY = 1.0f;

const int ROBOT_RADIUS = 35;

AStar::AStar()
{

}

/*!
* Sets the dimensions of the pitch. This will be defined in the shared memory, in terms of pixels as seen by the vision module.
*/
void AStar::SetSharedData(int pitchSizeX, int pitchSizeY, TPitchSide pitchSide)
{
	m_gridSizeX = pitchSizeX;
	m_gridSizeY = pitchSizeY;

	m_pitchSide = pitchSide;
}

std::list<RobotState> AStar::GeneratePath(RobotState startingState, RobotState destinationState, bool doWeHaveBall, Vector2 ballPos, RobotState enemyRobotFuture)
{
	Vector2 startingVector = startingState.Position();
	Vector2 destinationVector = destinationState.Position();

	std::list< std::pair<Vector2, AStarNode*> > openSet;
	std::list< std::pair<Vector2, AStarNode*> > closedSet;

	std::list< std::pair<Vector2, AStarNode*> >::iterator openSetIt;
	std::list< std::pair<Vector2, AStarNode*> >::iterator closedSetIt;

	m_costTravelled = 0;
	
	// First, let's add the starting node to the open set.
	AStarNode* p_startingNode = new AStarNode();
	openSet.push_front(std::make_pair(startingVector, p_startingNode));
	p_startingNode->setGScore(0.0f);
	p_startingNode->setHScore(startingVector.Distance(&destinationVector));

	// Start our timer.
	timeval startTimeStruct;
	gettimeofday(&startTimeStruct, NULL);
	
	double startTime = startTimeStruct.tv_sec+(startTimeStruct.tv_usec/1000000.0);

	// If the open set's not empty, that means we still have some nodes to explore.
	while (!openSet.empty())
	{
		Vector2 currentVector;
		AStarNode* currentNode;
		float lowestFScore = FLT_MAX;

		// Find the node in the open set with the lowest f-score. 
		// This is the node we want to expand next.
		for (openSetIt = openSet.begin(); openSetIt != openSet.end(); openSetIt++)
		{
			const float currentFScore = openSetIt->second->getFScore();

			if (currentFScore < lowestFScore)
			{
				currentVector = openSetIt->first;
				currentNode = openSetIt->second;
				lowestFScore = currentFScore;
			}
		}

		m_costTravelled = currentNode->getGScore();

		// Add the current node to the closed set & remove it from the open set.
		closedSet.push_front(std::make_pair(currentVector, currentNode));

		for (openSetIt = openSet.begin(); openSetIt != openSet.end(); openSetIt++)
		{
			if (openSetIt->first == currentVector)
			{
				openSet.erase(openSetIt);
				break;
			}
		}

		// Check if we've reached the goal node.
		// If we have, we've got all the information we need to produce the path.
		// Alternatively, can we terminate early here?

		if (currentVector == destinationVector)
		{
			// We now want to reconstruct the complete A* path.
			std::vector<Vector2> nodesOnPath;

			Vector2 previousVector = currentVector;

			while (previousVector != startingVector)
			{
				nodesOnPath.insert(nodesOnPath.begin(), previousVector);

				for (closedSetIt = closedSet.begin(); closedSetIt != closedSet.end(); closedSetIt++)
				{
					if (closedSetIt->first == previousVector)
					{
						previousVector = closedSetIt->second->getPreviousNode();
						break;
					}
				}
			}

			// DEBUG
			int openSetSize = openSet.size();
			int closedSetSize = closedSet.size();

			// Finally, add the starting node to the path.
			nodesOnPath.insert(nodesOnPath.begin(), startingVector);

			// DEBUG
			//std::ofstream myfile;
			//myfile.open("expanded_points_new.dat");

			for (openSetIt = openSet.begin(); openSetIt != openSet.end(); openSetIt++)
			{
				//myfile << openSetIt->first.ToString() << std::endl;

				delete(openSetIt->second);
				openSetIt->second = NULL;
			}

			for (closedSetIt = closedSet.begin(); closedSetIt != closedSet.end(); closedSetIt++)
			{
				//myfile << closedSetIt->first.ToString() << std::endl;

				delete(closedSetIt->second);
				closedSetIt->second = NULL;
			}

			//myfile.close();

			// statesOnPath describes both the positions and orientations on the path.
			std::list<RobotState> statesOnPath;

			for (int i=0; i<nodesOnPath.size(); i++)
			{
				float angleToNextPoint;

				// If this is the last point, we want to face the target direction.
				// Otherwise, just face towards the next point.
				if (i==0)
				{
					angleToNextPoint = startingState.Orientation();
				}
				else if (i >= nodesOnPath.size()-1)
				{
					angleToNextPoint = destinationState.Orientation();
				}
				else
				{
					angleToNextPoint = nodesOnPath[i].GetAngleTo(&nodesOnPath[i+1]);
				}

				statesOnPath.push_back(RobotState(nodesOnPath[i], angleToNextPoint));
			}
	
			return statesOnPath;
		}

		// Get all nodes adjacent to the current one (i.e. the nodes we can travel to from here).
		std::list<Vector2> adjacentNodes = FindAdjacentNodes(currentVector);
		std::list<Vector2>::iterator adjacentNodesIt;

		for (adjacentNodesIt = adjacentNodes.begin(); adjacentNodesIt != adjacentNodes.end(); adjacentNodesIt++)
		{
			Vector2 currentAdjacentVector = *adjacentNodesIt;

			bool isVectorInClosedSet = false;

			// Check if the node is in the closed set.
			for (closedSetIt=closedSet.begin(); closedSetIt != closedSet.end(); closedSetIt++)
			{
				if (closedSetIt->first == currentAdjacentVector)
				{
					isVectorInClosedSet = true;
				}
			}

			if (isVectorInClosedSet)
			{
				continue;
			}

			bool isVectorInOpenSet = false;
			
			for (openSetIt = openSet.begin(); openSetIt != openSet.end(); openSetIt++)
			{
				if (openSetIt->first == currentAdjacentVector)
				{
					// If we've improved upon a previously calculated cost to get to this node, update it.
					float newGScore = m_costTravelled + currentVector.Distance(&currentAdjacentVector);

					if (newGScore < openSetIt->second->getGScore())
					{
						openSetIt->second->setGScore(m_costTravelled + currentVector.Distance(&currentAdjacentVector));
						openSetIt->second->setHScore(currentAdjacentVector.Distance(&destinationVector) * HEURISTIC_PENALTY);
						openSetIt->second->setPreviousNode(currentVector);
					}
	
					isVectorInOpenSet = true;

					break;
				}
			}

			// If this node isn't in the open set, add it.
			if (!isVectorInOpenSet)
			{
				AStarNode* p_newAStarNode = new AStarNode();
				openSet.push_front(std::make_pair(currentAdjacentVector, p_newAStarNode));
				p_newAStarNode->setGScore(m_costTravelled + currentVector.Distance(&currentAdjacentVector));
				p_newAStarNode->setHScore(currentAdjacentVector.Distance(&destinationVector) * HEURISTIC_PENALTY);
				p_newAStarNode->setPreviousNode(currentVector);

				// We're dealing with a squared threshold so we don't have to bother with square roots.
				const float biasMinThresholdSquared = 250.0f;
				const float biasMaxThresholdSquared = 900.0f;
				const float defaultBias = 10000000.0f;

				// If we're moving to the ball and we want to get behind it, add bias in a U-shape around it
				// If we don't have the ball, we want to ensure that we come from behind it. 
				/*if (!doWeHaveBall)
				{
					float distanceToBallSquared = currentAdjacentVector.DistanceSquared(&ballPos);

					// Check proximity of the point first
					if (distanceToBallSquared <= biasMaxThresholdSquared)
					{
						if (m_pitchSide == eLeftSide)
						{
							// If we're on the left side of the pitch, the open side of the U needs to be that way.
							if ((currentAdjacentVector != ballPos) && ((currentAdjacentVector.X() >= ballPos.X()) || (pow(fabs(currentAdjacentVector.Y() - ballPos.Y()),2) > biasMinThresholdSquared)))
							{
								// Let's apply a radial bias.
								float biasToApply = defaultBias/distanceToBallSquared;

								p_newAStarNode->setBias(biasToApply);
							}
						}
						else
						{
							// If we're on the right side of the pitch, the open side of the U needs to be that way.
							if ((currentAdjacentVector != ballPos) && ((currentAdjacentVector.X() <= ballPos.X()) || (pow(fabs(currentAdjacentVector.Y() - ballPos.Y()),2) > biasMinThresholdSquared)))
							{
								// Let's apply a radial bias.
								float biasToApply = defaultBias/distanceToBallSquared;

								p_newAStarNode->setBias(biasToApply);
							}
						}
					}
				}*/

				// Add bias to a radius around the enemy robot.
				Vector2 enemyRobotPosition = enemyRobotFuture.Position();

				float distanceToEnemyRobotSqd = currentAdjacentVector.DistanceSquared(&enemyRobotPosition);
				float robotRadiusSquared = pow(2*ROBOT_RADIUS, 2);

				if (distanceToEnemyRobotSqd < robotRadiusSquared)
				{
					p_newAStarNode->setBias(defaultBias / distanceToEnemyRobotSqd);
				}
			}
		}

		// Update our timer.
		timeval currentTimeStruct;
		gettimeofday(&currentTimeStruct, NULL);
		double currentTime = currentTimeStruct.tv_sec+(currentTimeStruct.tv_usec/1000000.0);

		// Check if this A* pass has been running for too long.
		double timeTaken = currentTime - startTime;

		if (timeTaken >= TIMER_EXPIRY)
		{
			std::string logMessage = "AI: The A* time limit has expired.";
			loggingObj->ShowMsg(logMessage.c_str());

			std::list<RobotState> blankList;
			return blankList;
		}
	}

	// If we get here, we've tried all points, but haven't been able to get 
	// from the starting point to the destination.
	for (openSetIt = openSet.begin(); openSetIt != openSet.end(); openSetIt++)
	{
		delete(openSetIt->second);
		openSetIt->second = NULL;
	}

	for (closedSetIt = closedSet.begin(); closedSetIt != closedSet.end(); closedSetIt++)
	{
		delete(closedSetIt->second);
		closedSetIt->second = NULL;
	}

	std::list<RobotState> blankList;
	return blankList;
}

// Find all of the nodes adjacent to the supplied one. Maximum of 8.
std::list<Vector2> AStar::FindAdjacentNodes(Vector2 currentNode)
{
	std::list<Vector2> adjacentNodes;

	for (int i=-1; i<=1; i++)
	{
		for (int j=-1; j<=1; j++)
		{
			// Check that the node is valid.

			// In this case, this is the current node.
			if ((i==0) && (j==0))
			{
				continue;
			}
			
			// In the following two cases, the node is off the edge of the pitch.
			if ((currentNode.X() + i < 0) || (currentNode.Y() + j < 0))
			{
				continue;
			}

			if ((currentNode.X() + i >= m_gridSizeX) || (currentNode.Y() + j >= m_gridSizeY))
			{
				continue;
			}

			// If we're here, the node is valid and can be added to the list.
			adjacentNodes.push_back(Vector2(currentNode.X() + i, currentNode.Y() + j));
		}
	}

	return adjacentNodes;
}
