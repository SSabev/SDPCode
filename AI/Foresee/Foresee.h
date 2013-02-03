#if !defined(FORESEE_H)
#define FORESEE_H

#include "../MathTypes/Vector2.h"

#include <list>
#include <vector>

class Foresee
{
#if defined(TEST)
	// This facilitates testing of private and protected functions.
	friend class AStarUnitTests;
#endif

public:
	Foresee();
	
	std::vector<Vector2> ExtrapolateState(Vector2 ourRobotPos, Vector2 enemyRobotPos, Vector2 ballPos);
	
private: 
	Vector2 ExtrapolatePositionFromPoints(std::vector<Vector2> positions);

	std::list<Vector2> m_ourRobotPositions;
	std::list<Vector2> m_enemyRobotPositions;
	std::list<Vector2> m_ballPositions;
};

#endif