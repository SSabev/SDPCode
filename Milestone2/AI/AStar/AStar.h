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
	
	void SetPitchDimensions(int pitchSizeX, int pitchSizeY);
	std::list<Vector2> GeneratePath(Vector2 startingNode, Vector2 destinationNode);

private:
	std::list<Vector2> FindAdjacentNodes(Vector2 currentNode);
	bool CanTerminateEarly();

	float m_costTravelled;

	int m_gridSizeX;
	int m_gridSizeY;
};

#endif