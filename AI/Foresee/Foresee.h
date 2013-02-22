#if !defined(FORESEE_H)
#define FORESEE_H

#include "../MathTypes/Vector2.h"
#include "../RobotState.h"

#include <list>
#include <vector>

class Foresee
{
#if defined(STANDALONE)
	// This facilitates testing of private and protected functions.
	friend class ForeseeUnitTests;
#endif

public:
	Foresee();
	
	void ExtrapolateEnvironment(RobotState ourRobotCurrent, RobotState enemyRobotCurrent, Vector2 ballCurrent, RobotState &ourRobotFuture, RobotState &enemyRobotFuture, Vector2 &ballFuture);
	void SetPitchDimensions(int pitchSizeX, int pitchSizeY);
	
private: 
	RobotState ExtrapolateState(std::vector<RobotState> states);
	Vector2 ExtrapolatePosition(std::vector<Vector2> positions);
	
	std::vector<RobotState> m_ourRobotStates;
	std::vector<RobotState> m_enemyRobotStates;
	std::vector<Vector2> m_ballPositions;

	int m_pitchSizeX;
	int m_pitchSizeY;
};

#endif
