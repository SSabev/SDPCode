#if !defined(FORESEE_H)
#define FORESEE_H

#include "../MathTypes/Vector2.h"
#include "../RobotState.h"

#include <list>
#include <vector>

class Foresee
{
#if defined(TEST)
	// This facilitates testing of private and protected functions.
	friend class ForeseeUnitTests;
#endif

public:
	Foresee();
	
	void ExtrapolateState(RobotState ourRobotPos, RobotState enemyRobotPos, Vector2 ballPos, RobotState &ourRobotFuture, RobotState &enemyRobotFuture, Vector2 &ballFuture);
	void SetPitchDimensions(int pitchSizeX, int pitchSizeY);
	
private: 
	Vector2 ExtrapolatePositionFromPoints(std::vector<Vector2> positions);
	float ExtrapolateAngle(std::vector<float> angles);

	std::list<Vector2> m_ourRobotPositions;
	std::list<float> m_ourRobotAngles;
	std::list<Vector2> m_enemyRobotPositions;
	std::list<float> m_enemyRobotAngles;
	std::list<Vector2> m_ballPositions;

	int m_pitchSizeX;
	int m_pitchSizeY;
};

#endif
