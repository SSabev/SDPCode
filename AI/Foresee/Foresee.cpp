#include "Foresee.h"

#include <assert.h>

const int POSITIONSTOKEEP = 10;

Foresee::Foresee()
{
	
}

/*!
* Sets the dimensions of the pitch. This will be defined in the shared memory, in terms of pixels as seen by the vision module.
*/
void Foresee::SetPitchDimensions(int pitchSizeX, int pitchSizeY)
{
	m_pitchSizeX = pitchSizeX;
	m_pitchSizeY = pitchSizeY;
}

/*!
 * Extrapolates to a predicated future state based on the current and previous states. The purpose of this is to 
 * ascertain the position which the robot is likely to be in when it receives the instructions calculated in 
 * this pass.
 *
 * Note that if this is the first run of this method and there is no previous data, extrapolation cannot be 
 * performed and the current state will be returned.
*/
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

/*!
 * Extrapolates a future position based on the supplied set of points. The points should be supplied in reverse 
 * chronological order. 
 *
 * If only one point is supplied, extrapolation cannot be performed and this point will be returned.
*/
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

/*!
 * Extrapolates a future angle based on the supplied set of angles. The angles should be supplied in 
 * reverse chronological order. 
 *
 * If only one angle is supplied, extrapolation cannot be performed and this angle will be returned.
*/
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