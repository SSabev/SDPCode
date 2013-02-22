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
void Foresee::ExtrapolateEnvironment(RobotState ourRobotCurrent, RobotState enemyRobotCurrent, Vector2 ballCurrent, RobotState &ourRobotFuture, RobotState &enemyRobotFuture, Vector2 &ballFuture)
{
	// Limit the amount of positions we're keeping.
	if (m_ourRobotStates.size() >= POSITIONSTOKEEP)
	{
		m_ourRobotStates.pop_back();
	}
	
	if (m_enemyRobotStates.size() >= POSITIONSTOKEEP)
	{
		m_enemyRobotStates.pop_back();
	}
	
	if (m_ballPositions.size() >= POSITIONSTOKEEP)
	{
		m_ballPositions.pop_back();
	}
	
	m_ourRobotStates.insert(m_ourRobotStates.begin(),ourRobotCurrent);
	m_enemyRobotStates.insert(m_enemyRobotStates.begin(),enemyRobotCurrent);
	m_ballPositions.insert(m_ballPositions.begin(),ballCurrent);
	
	ourRobotFuture = ExtrapolateRobotState(m_ourRobotStates);
	enemyRobotFuture = ExtrapolateRobotState(m_enemyRobotStates);
	ballFuture = ExtrapolatePosition(m_ballPositions);
}

/*!
 * Extrapolates a future state based on the supplied set of states. The states should be supplied in reverse 
 * chronological order. 
 *
 * If only one state is supplied, extrapolation cannot be performed and this state will be returned.
*/
RobotState Foresee::ExtrapolateRobotState(std::vector<RobotState> states)
{
	assert(states.size() > 0);
	
	// If the list of positions only contains one entry, there's not 
	// enough data to extrapolate from.
	if (states.size() == 1)
	{
		return states.front();
	}
	
	// For now, we're going to assume that the delay between frames is constant.
	Vector2 extrapolatedPosition = states[0].Position() + (states[0].Position() - states[1].Position());

	// Ensure that the extrapolated position is within the confines of the pitch.
	extrapolatedPosition.Clamp(Vector2(0,0), Vector2(m_pitchSizeX-1, m_pitchSizeY-1));

	float extrapolatedAngle = states[0].Orientation() + (states[0].Orientation() - states[1].Orientation());
	
	RobotState extrapolatedState(extrapolatedPosition, extrapolatedAngle);

	return extrapolatedState;
}

/*!
 * Extrapolates a future position based on the supplied set of points. The points should be supplied in reverse 
 * chronological order. 
 *
 * If only one point is supplied, extrapolation cannot be performed and this point will be returned.
*/
Vector2 Foresee::ExtrapolatePosition(std::vector<Vector2> positions)
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