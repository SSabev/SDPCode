#if !defined(ASTAR_H)
#define ASTAR_H

#include "../MathTypes/Vector2.h"
#include "AStarNode.h"

#include <list>

class AStar
{

#if defined(TEST)
	// This facilitates testing of private and protected functions.
	friend class AStarUnitTests;
#endif

public:
	AStar();
	
	std::list<Vector2> GeneratePath(Vector2 startingNode, Vector2 destinationNode);

private:
	std::list<Vector2> FindAdjacentNodes(Vector2 currentNode);

	float m_costTravelled;
	std::list< std::pair<Vector2, AStarNode*> > m_openSet;
	std::list< std::pair<Vector2, AStarNode*> > m_closedSet;
};

#endif