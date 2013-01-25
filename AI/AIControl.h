#if !defined(AICONTROL_H)
#define AICONTROL_H

#include "../Shared/SharedMem.h"

#include "MathTypes/Vector2.h"
#include "RobotState.h"

#include "Foresee/Foresee.h"
#include "Eagle/Eagle.h"
#include "AStar/AStar.h"
#include "Impala/Impala.h"

#include <list>

class AIControl
{
public:
	void Initialise();
	void RunAI();
	
private:
	Foresee m_foresee;
	Eagle m_eagle;
	AStar m_aStar;
	Impala m_impala;
};

#endif