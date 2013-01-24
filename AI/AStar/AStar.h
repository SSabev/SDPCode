#if !defined(ASTAR_H)
#define ASTAR_H

#include "MathTypes/Vector2.h"
#include "AStarNode.h"

#include <vector>
#include <map>

class AStar
{
public:
	AStar();
	
	std::vector<Vector2> GeneratePath(Vector2 currentPosition, Vector2 destination);

private:
	std::map<Vector2, AStarNode*> m_openSet;
	std::map<Vector2, AStarNode*> m_closedSet;
};

#endif