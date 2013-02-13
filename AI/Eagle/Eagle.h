#if !defined(EAGLE_H)
#define EAGLE_H

#include "../RobotState.h"
#include "../MathTypes/Vector2.h"

#include "../Shared/SharedMem.h"

#include <vector>

class Eagle
{
public:
	Eagle();
	
	void SetState(TSystemState state);
	RobotState IdentifyTarget(RobotState ourRobotState, RobotState enemyRobotState, Vector2 ballPos);

private:
	bool m_hasReachedBall;
	TSystemState m_state;
};

#endif