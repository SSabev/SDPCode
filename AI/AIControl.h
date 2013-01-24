#if !defined(AICONTROL_H)
#define AICONTROL_H

#include "Foresee.h"
#include "Eagle.h"
#include "AStar.h"
#include "Impala.h"

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