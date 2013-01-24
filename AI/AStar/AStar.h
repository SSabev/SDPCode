#if !defined(ASTAR_H)
#define ASTAR_H

#include "MathTypes/Vector2.h"
#include "AStarNode.h"

#include <vector>
#include <list>
#include <map>

class AStar
{
public:
	AStar();
	
	std::vector<Vector2> GeneratePath(Vector2 startingNode, Vector2 destinationNode);

private:
	std::list<Vector2> FindAdjacentNodes(Vector2 currentNode);

	std::map<Vector2, AStarNode*, Vector2Comparer> m_openSet;
	std::map<Vector2, AStarNode*, Vector2Comparer> m_closedSet;
};

#endif