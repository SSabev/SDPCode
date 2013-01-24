#if !defined(ASTAR_H)
#define ASTAR_H

#include "MathTypes/Vector2.h"

#include <vector>

class AStar
{
public:
	AStar();
	
	std::vector<Vector2> GeneratePath(Vector2 currentPosition, Vector2 destination);
};

#endif