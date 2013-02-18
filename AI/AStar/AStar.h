#if !defined(ASTAR_H)
#define ASTAR_H

#include "../RobotState.h"
#include "../MathTypes/Vector2.h"
#include "AStarNode.h"

#include "../../Shared/SharedMem.h"

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
	void SetState(TSystemState state);
	std::list<RobotState> GeneratePath(RobotState startingState, RobotState destinationState);

private:
	std::list<Vector2> FindAdjacentNodes(Vector2 currentNode);
	bool CanTerminateEarly();

	float m_costTravelled;
	
	TSystemState m_state;

	int m_gridSizeX;
	int m_gridSizeY;

	Vector2 m_ballPos;
};

#endif
