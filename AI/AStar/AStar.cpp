#include "AStar.h"

#include <float.h>

const int GRID_SIZE_X = 50;
const int GRID_SIZE_Y = 20;

AStar::AStar()
{

}

std::vector<Vector2> AStar::GeneratePath(Vector2 startingNode, Vector2 destinationNode)
{
	// First, let's add the starting node to the open set.
	m_openSet[startingNode] = new AStarNode();
	m_openSet[startingNode]->setGScore(0.0f);
	m_openSet[startingNode]->setHScore(startingNode.Distance(&destinationNode));

	// If the open set's not empty, that means we still have some nodes to explore.
	while (!m_openSet.empty())
	{
		Vector2 currentNode;
		float lowestFScore = FLT_MAX;

		std::map<Vector2, AStarNode*, Vector2Comparer>::iterator it;

		// Find the node in the open set with the lowest f-score. 
		// This is the node we want to expand next.
		for (it = m_openSet.begin(); it != m_openSet.end(); it++)
		{
			const float currentFScore = it->second->getFScore();

			if (currentFScore < lowestFScore)
			{
				currentNode = it->first;
			}
		}

		// Check if we've reached the goal node.
		if (currentNode == destinationNode)
		{

		}
	}
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
}