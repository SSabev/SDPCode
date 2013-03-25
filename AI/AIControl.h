#if !defined(AICONTROL_H)
#define AICONTROL_H

#include "../Shared/SharedMem.h"
#include "../Shared/Logging.h"

#include "MathTypes/Vector2.h"
#include "RobotState.h"

#include "Foresee/Foresee.h"
#include "Eagle/Eagle.h"
#include "AStar/AStar.h"
#include "Impala/Impala.h"

#include <list>

class AIControl
{

#if defined(STANDALONE)
	// This facilitates testing of private and protected functions.
	friend class AStarUnitTests;
	friend class AIControlUnitTests;
#endif

public:
	void Initialise();

#if defined(STANDALONE)
	void Initialise(TShMem* pSharedMemory);
#endif

	void RunAI(TAIEntry* aiEntry);
	
private:
	bool CoordinatesAreBad(Vector2 objectPosition);
	bool IsFailedFrame(RobotState ourRobot);
	float WrapValue(float orientation);

#if defined(STANDALONE)
	void Plot(std::list<RobotState> aStarPath, std::vector<RobotState> ourPrevious, RobotState destination, std::vector<Vector2> ballPrevious, Vector2 ballFuture, std::vector<RobotState> enemyRobotPrevious, RobotState enemyRobotFuture);
#endif

	Foresee m_foresee;
	Eagle m_eagle;
	AStar m_aStar;
	Impala m_impala;
	bool m_isLastKnownBallPosSet;
	bool m_isLastKnownRobotStateSet;

	Vector2 m_lastKnownBallPosition;
	RobotState m_lastKnownRobotState;
	bool m_hadBallLastFrame;

#if defined(STANDALONE)
	// Simulate the shared memory if we're running standalone.
	TShMem* m_pSharedMemory;
	ILogging* loggingObj;
#endif
};

#endif
