#if !defined(EAGLE_H)
#define EAGLE_H

#include "../RobotState.h"
#include "../MathTypes/Vector2.h"
#include "../MathTypes/Intersection.h"

#include "../../Shared/SharedMem.h"

#include <vector>

class Eagle
{
friend class EagleUnitTests;	
public:
	Eagle();
	
	void SetSharedData(TSystemState state, int pitchSizeX, int pitchSizeY, TPitchSide pitchSide);
	RobotState IdentifyTarget(RobotState ourRobotState, RobotState enemyRobotState, Vector2 ballPos);
	bool ShouldWeShoot(RobotState ourRobotState, RobotState enemyRobotState, Vector2 ballPos);
	bool DoWeHaveBall(RobotState ourRobotState, Vector2 ballPos); 

private:
	Vector2 GoalCentrePosition();
	TSystemState m_state;
	TPitchSide m_pitchSide;

	int m_pitchSizeX;
	int m_pitchSizeY;

	Intersection m_intersection;
};

#endif
