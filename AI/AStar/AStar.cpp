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
			AStarNode* aStarNode = new AStarNode();

			m_openSet.insert(std::pair(Vector2(x,y), aStarNode));
		}
	}
 
}

std::vector<Vector2> AStar::GeneratePath(Vector2 currentPosition, Vector2 destination)
{
	
}