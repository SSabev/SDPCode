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

#if defined(TEST)
	// This facilitates testing of private and protected functions.
	friend class AStarUnitTests;
#endif

public:
	void Initialise();
	void RunAI();
	
private:
	void Plot(std::list<Vector2> aStarPath, std::vector<Vector2> ourPrevious, Vector2 destination, std::vector<Vector2> ballPrevious, Vector2 ballFuture);

	Foresee m_foresee;
	Eagle m_eagle;
	AStar m_aStar;
	Impala m_impala;

#if defined(STANDALONE)
	// Simulate the shared memory if we're running standalone.
	TShMem sharedMem;
	ILogging* loggingObj;
#endif
};

#endif