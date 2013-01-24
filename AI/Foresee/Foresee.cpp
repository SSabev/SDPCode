#include "Foresee.h"

#include <assert.h>

const int POSITIONSTOKEEP = 10;

Foresee::Foresee()
{
	
}

std::vector<Vector2> Foresee::ExtrapolateState(Vector2 ourRobotPos, Vector2 enemyRobotPos, Vector2 ballPos)
{
	// Limit the amount of positions we're keeping.
	if (m_ourRobotPositions.size() >= POSITIONSTOKEEP)
	{
		m_ourRobotPositions.pop_back();
	}
	
	if (m_enemyRobotPositions.size() >= POSITIONSTOKEEP)
	{
		m_enemyRobotPositions.pop_back();
	}
	
	if (m_ballPositions.size() >= POSITIONSTOKEEP)
	{
		m_ballPositions.pop_back();
	}
	
	m_ourRobotPositions.push_front(ourRobotPos);
	m_enemyRobotPositions.push_front(enemyRobotPos);
	m_ballPositions.push_front(ballPos);
	
	std::vector<Vector2> futurePositions;
	futurePositions[0] = ExtrapolatePositionFromPoints(m_ourRobotPositions);
	futurePositions[1] = ExtrapolatePositionFromPoints(m_enemyRobotPositions);
	futurePositions[2] = ExtrapolatePositionFromPoints(m_ballPositions);
	
	return futurePositions;
}

Vector2 Foresee::ExtrapolatePositionFromPoints(std::list<Vector2> positions)
{
	assert(positions.size() > 0);
	
	// If the list of positions only contains one entry, there's not 
	// enough data to extrapolate from.
	if (positions.size() == 1)
	{
		return positions.front();
	}
	
	
}