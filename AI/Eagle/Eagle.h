#if !defined(EAGLE_H)
#define EAGLE_H

#include "../RobotState.h"
#include "../MathTypes/Vector2.h"

#include "../../Shared/SharedMem.h"

#include <vector>

class Eagle
{
friend class EagleUnitTests;	
public:
	Eagle();
	
	void SetState(TSystemState state);
	void SetPitchDimensions(int pitchSizeX, int pitchSizeY);
	RobotState IdentifyTarget(RobotState ourRobotState, RobotState enemyRobotState, Vector2 ballPos);

private:
	bool DoWeHaveBall(RobotState ourRobotState, Vector2 ballPos); 

	TSystemState m_state;

	int m_pitchSizeX;
	int m_pitchSizeY;
};

#endif
