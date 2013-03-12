#if !defined(ASTAR_H)
#define ASTAR_H

#include "../RobotState.h"
#include "../MathTypes/Vector2.h"
#include "AStarNode.h"

#include "../../Shared/SharedMem.h"
#include "../../Shared/Logging.h"

#include <list>

class AStar
{

#if defined(STANDALONE)
	// This facilitates testing of private and protected functions.
	friend class AStarUnitTests;
#endif

public:
	AStar();
	
	void SetSharedData(int pitchSizeX, int pitchSizeY, TPitchSide pitchSide);
	std::list<RobotState> GeneratePath(RobotState startingState, RobotState destinationState, bool doWeHaveBall, Vector2 ballPos, RobotState enemyRobotFuture);

private:
	std::list<Vector2> FindAdjacentNodes(Vector2 currentNode);

	float m_costTravelled;
	
	TSystemState m_state;

	int m_gridSizeX;
	int m_gridSizeY;
	TPitchSide m_pitchSide;

	Vector2 m_ballPos;

	#if defined(STANDALONE)
	// Simulate the logging object if we're running standalone.
	ILogging* loggingObj;
#endif
};

#endif
