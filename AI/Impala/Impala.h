#if !defined(IMPALA_H)
#define IMPALA_H

#include "../MathTypes/Vector2.h"
#include "../RobotState.h"

#include "../../Shared/SharedMem.h"

#include <list>
#include <vector>

class Impala
{
public:
	Impala();
	
    	void SetSharedData(TSystemState state, int pitchSizeX, int pitchSizeY, TPitchSide pitchSide);
	std::list<RobotState> SmoothPath(std::list<RobotState> aStarPath, bool doWeHaveBall, int numAdditionalPasses=0);


private:
 	Vector2 GoalCentrePosition();
  	TSystemState m_state;
	TPitchSide m_pitchSide;

	int m_pitchSizeX;
	int m_pitchSizeY;
	std::list<Vector2> RunPass(std::list<Vector2> path);
	std::vector<Vector2> CollapsePoints(std::list<Vector2> path);
};

#endif
