#if !defined(ASTAR_H)
#define ASTAR_H

#include "../MathTypes/Vector2.h"
#include "AStarNode.h"

#include <list>
#include <map>

#if defined(TEST)
// This facilitates testing of private and protected functions.
#include "../UnitTests/AStarUnitTests.cpp"
friend class AStarUnitTests
#endif

class AStar
{
public:
	AStar();
	
	std::list<Vector2> GeneratePath(Vector2 startingNode, Vector2 destinationNode);

private:
	std::list<Vector2> FindAdjacentNodes(Vector2 currentNode);

	float m_costTravelled;
	std::map<Vector2, AStarNode*, Vector2Comparer> m_openSet;
	std::map<Vector2, AStarNode*, Vector2Comparer> m_closedSet;
};

#endif