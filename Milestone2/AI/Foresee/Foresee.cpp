#include "Foresee.h"

#include <assert.h>

const int POSITIONSTOKEEP = 10;

Foresee::Foresee()
{
	
}

void Foresee::SetPitchDimensions(int pitchSizeX, int pitchSizeY)
{
	m_pitchSizeX = pitchSizeX;
	m_pitchSizeY = pitchSizeY;
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
    std::vector<Vector2> enemyRobotVector(m_enemyRobotPositions.begin(), m_enemyRobotPositions.end());
    std::vector<Vector2> ballVector(m_ballPositions.begin(), m_ballPositions.end());
	
	std::vector<Vector2> futurePositions;
    futurePositions.push_back(ExtrapolatePositionFromPoints(ourRobotVector));
    futurePositions.push_back(ExtrapolatePositionFromPoints(enemyRobotVector));
    futurePositions.push_back(ExtrapolatePositionFromPoints(ballVector));
	
	return futurePositions;
}

Vector2 Foresee::ExtrapolatePositionFromPoints(std::vector<Vector2> positions)
{
	assert(positions.size() > 0);
	
	// If the list of positions only contains one entry, there's not 
	// enough data to extrapolate from.
	if (positions.size() == 1)
	{
        return positions[0];
	}
	
	// For now, we're going to assume that the delay between frames is constant.
	Vector2 extrapolatedPosition = positions[0] + (positions[0] - positions[1]);

	// Ensure that the extrapolated position is within the confines of the pitch.
	extrapolatedPosition.Clamp(Vector2(0,0), Vector2(m_pitchSizeX-1, m_pitchSizeY-1));
	
	return extrapolatedPosition;
}
