#include "AStar.h"

const int GRID_SIZE_X = 50;
const int GRID_SIZE_Y = 20;

AStar::AStar()
{
	// Create all the nodes for our open set.
	for (int x=0; x<GRID_SIZE_X; x++)
	{	
		for (int y=0; y<GRID_SIZE_Y; y++)
		{
			Vector2 nodePosition(x,y);
			AStarNode* aStarNode = new AStarNode();

			m_openSet[nodePosition] = aStarNode;
		}
	}
 
}

std::vector<Vector2> AStar::GeneratePath(Vector2 startingNode, Vector2 destinationNode)
{
	// Work out which nodes we 
}

// Find all of the nodes adjacent to the supplied one. Maximum of 8.
std::list<AStarNode*> AStar::FindAdjacentNodes(Vector2 currentNode)
{

}