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
#endif

public:
	void Initialise();

#if defined(STANDALONE)
	void Initialise(TShMem* pSharedMemory);
#endif

	void RunAI();
	
private:
	bool CoordinatesAreBad(Vector2 objectPosition);
	bool IsFailedFrame(RobotState ourRobot, Vector2 ball);

#if defined(TEST)
	void Plot(std::list<RobotState> aStarPath, std::vector<RobotState> ourPrevious, RobotState destination, std::vector<Vector2> ballPrevious, Vector2 ballFuture, std::vector<RobotState> enemyRobotPrevious, RobotState enemyRobotFuture);
#endif

	Foresee m_foresee;
	Eagle m_eagle;
	AStar m_aStar;
	Impala m_impala;

#if defined(STANDALONE)
	// Simulate the shared memory if we're running standalone.
	TShMem* m_pSharedMemory;
	ILogging* loggingObj;
#endif
};

#endif