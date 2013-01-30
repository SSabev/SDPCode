#include "AStar.h"

#include <float.h>
#include <assert.h>
#include <iostream>

const int GRID_SIZE_X = 244;
const int GRID_SIZE_Y = 122;
// I'm experimenting with penalising the heuristic, otherwise it results in expanding 
// too many nodes needlessly.
// This will make it inadmissable (not really A* anymore), but given that we know there are 
// few obstacles on the pitch and a truly optimal path isn't really necessary, we can 
// probably get away with it. **The speed benefit is insane - factor of 100s**
const float HEURISTIC_PENALTY = 1.5;

AStar::AStar()
{

}

std::list<Vector2> AStar::GeneratePath(Vector2 startingVector, Vector2 destinationVector)
{
	m_costTravelled = 0;
	
	// First, let's add the starting node to the open set.
	AStarNode* p_startingNode = new AStarNode();
	m_openSet.push_front(std::make_pair(startingVector, p_startingNode));
	p_startingNode->setGScore(0.0f);
	p_startingNode->setHScore(startingVector.Distance(&destinationVector));

	// If the open set's not empty, that means we still have some nodes to explore.
	while (!m_openSet.empty())
	{
		Vector2 currentVector;
		AStarNode* currentNode;
		float lowestFScore = FLT_MAX;

		std::list< std::pair<Vector2, AStarNode*> >::iterator openSetIt;
		std::list< std::pair<Vector2, AStarNode*> >::iterator closedSetIt;

		//std::cout << "OpenSet" << std::endl;

		// Find the node in the open set with the lowest f-score. 
		// This is the node we want to expand next.
		for (openSetIt = m_openSet.begin(); openSetIt != m_openSet.end(); openSetIt++)
		{
			// DEBUG
			//std::cout << openSetIt->first.ToString() << std::endl;

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
		m_closedSet.push_front(std::make_pair(currentVector, currentNode));

		for (openSetIt = m_openSet.begin(); openSetIt != m_openSet.end(); openSetIt++)
		{
			if (openSetIt->first == currentVector)
			{
				m_openSet.erase(openSetIt);
				break;
			}
		}

		int debugOpenSetSize = m_openSet.size();
		int debugClosedSetSize = m_closedSet.size();

		// Check if we've reached the goal node.
		// If we have, we've got all the information we need to produce the path.
		if (currentVector == destinationVector)
		{
			// We now want to reconstruct the complete A* path.
			std::list<Vector2> nodesOnPath;

			Vector2 previousVector = destinationVector;

			while (previousVector != startingVector)
			{
				nodesOnPath.push_front(previousVector);

				int debugClosedSetSize = m_closedSet.size();
				int debugNodesOnPathSize = nodesOnPath.size();

				for (closedSetIt = m_closedSet.begin(); closedSetIt != m_closedSet.end(); closedSetIt++)
				{
					if (closedSetIt->first == previousVector)
					{
						previousVector = closedSetIt->second->getPreviousNode();
						break;
					}
				}
			}

			// Finally, add the starting node to the path.
			nodesOnPath.push_front(startingVector);

			return nodesOnPath;
		}

		// Get all nodes adjacent to the current one (i.e. the nodes we can travel to from here).
		std::list<Vector2> adjacentNodes = FindAdjacentNodes(currentVector);
		std::list<Vector2>::iterator adjacentNodesIt;

		for (adjacentNodesIt = adjacentNodes.begin(); adjacentNodesIt != adjacentNodes.end(); adjacentNodesIt++)
		{
			Vector2 currentAdjacentVector = *adjacentNodesIt;

			bool isVectorInClosedSet = false;

			// Check if the node is in the closed set.
			for (closedSetIt=m_closedSet.begin(); closedSetIt != m_closedSet.end(); closedSetIt++)
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
			
			for (openSetIt = m_openSet.begin(); openSetIt != m_openSet.end(); openSetIt++)
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
				m_openSet.push_front(std::make_pair(currentAdjacentVector, p_newAStarNode));
				p_newAStarNode->setGScore(m_costTravelled + currentVector.Distance(&currentAdjacentVector));
				p_newAStarNode->setHScore(currentAdjacentVector.Distance(&destinationVector) * HEURISTIC_PENALTY);
				p_newAStarNode->setPreviousNode(currentVector);
			}
		}
	}

	assert(false);
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

			if ((currentNode.X() + i >= GRID_SIZE_X) || (currentNode.Y() + j >= GRID_SIZE_Y))
			{
				continue;
			}

			// If we're here, the node is valid and can be added to the list.
			adjacentNodes.push_back(Vector2(currentNode.X() + i, currentNode.Y() + j));
		}
	}

	return adjacentNodes;
}