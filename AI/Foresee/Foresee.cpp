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

	std::vector<Vector2> ourRobotVector(m_ourRobotPositions.begin(), m_ourRobotPositions.end());
	std::vector<Vector2> enemyRobotVector(m_enemyRobotPositions.begin(), m_ourRobotPositions.end());
	std::vector<Vector2> ballVector(m_ourRobotPositions.begin(), m_ballPositions.end());
	
	std::vector<Vector2> futurePositions;
	futurePositions[0] = ExtrapolatePositionFromPoints(ourRobotVector);
	futurePositions[1] = ExtrapolatePositionFromPoints(enemyRobotVector);
	futurePositions[2] = ExtrapolatePositionFromPoints(ballVector);
	
	return futurePositions;
}

Vector2 Foresee::ExtrapolatePositionFromPoints(std::vector<Vector2> positions)
{
	assert(positions.size() > 0);
	
	// If the list of positions only contains one entry, there's not 
	// enough data to extrapolate from.
	if (positions.size() == 1)
	{
		return positions.front();
	}
	
	// For now, we're going to assume that the delay between frames is constant.
	Vector2 extrapolatedPosition = positions[0] + (positions[0] - positions[1]);
	
	return extrapolatedPosition;
}