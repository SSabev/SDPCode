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

void Foresee::ExtrapolateState(RobotState ourRobotPos, RobotState enemyRobotPos, Vector2 ballPos, RobotState &ourRobotFuture, RobotState &enemyRobotFuture, Vector2 &ballFuture)
{
	// Limit the amount of positions we're keeping.
	if (m_ourRobotPositions.size() >= POSITIONSTOKEEP)
	{
		m_ourRobotPositions.pop_back();
	}
	
	if (m_ourRobotAngles.size() >= POSITIONSTOKEEP)
	{
		m_ourRobotAngles.pop_back();
	}
	
	if (m_enemyRobotPositions.size() >= POSITIONSTOKEEP)
	{
		m_enemyRobotPositions.pop_back();
	}

	if (m_enemyRobotAngles.size() >= POSITIONSTOKEEP)
	{
		m_enemyRobotAngles.pop_back();
	}
	
	if (m_ballPositions.size() >= POSITIONSTOKEEP)
	{
		m_ballPositions.pop_back();
	}
	
	m_ourRobotPositions.push_front(ourRobotPos.Position());
	m_ourRobotAngles.push_front(ourRobotPos.Orientation());
	m_enemyRobotPositions.push_front(enemyRobotPos.Position());
	m_enemyRobotAngles.push_front(enemyRobotPos.Orientation());
	m_ballPositions.push_front(ballPos);

	std::vector<Vector2> ourRobotVector(m_ourRobotPositions.begin(), m_ourRobotPositions.end());
	std::vector<float> ourRobotAnglesVector(m_ourRobotAngles.begin(), m_ourRobotAngles.end());
	std::vector<Vector2> enemyRobotVector(m_enemyRobotPositions.begin(), m_enemyRobotPositions.end());
	std::vector<float> enemyRobotAnglesVector(m_enemyRobotAngles.begin(), m_enemyRobotAngles.end());
	std::vector<Vector2> ballVector(m_ballPositions.begin(), m_ballPositions.end());
	
	ourRobotFuture.SetPosition(ExtrapolatePositionFromPoints(ourRobotVector));
	enemyRobotFuture.SetPosition(ExtrapolatePositionFromPoints(enemyRobotVector));
	ballFuture = ExtrapolatePositionFromPoints(ballVector);

	ourRobotFuture.SetOrientation(ExtrapolateAngle(ourRobotAnglesVector));
	enemyRobotFuture.SetOrientation(ExtrapolateAngle(enemyRobotAnglesVector));
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

	// Ensure that the extrapolated position is within the confines of the pitch.
	extrapolatedPosition.Clamp(Vector2(0,0), Vector2(m_pitchSizeX-1, m_pitchSizeY-1));
	
	return extrapolatedPosition;
}

float Foresee::ExtrapolateAngle(std::vector<float> angles)
{
	assert(angles.size() > 0);

	// If the list of angles only contains one entry, there's not 
	// enough data to extrapolate from.
	if (angles.size() == 1)
	{
		return angles.front();
	}

	float extrapolatedAngle = angles[0] + (angles[0] - angles[1]);

	return extrapolatedAngle;
}