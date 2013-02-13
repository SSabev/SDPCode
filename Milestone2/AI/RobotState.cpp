#include "RobotState.h"

RobotState::RobotState()
{
	m_isPositionSet = false;
	m_isOrientationSet = false;
}

RobotState::RobotState(float posX, float posY, float orientation)
{
	m_position.Set(posX, posY);
	m_orientation = orientation;

	m_isPositionSet = true;
	m_isOrientationSet = true;
}

void RobotState::SetPosition(float posX, float posY)
{
	m_position.Set(posX, posY);

	m_isPositionSet = true;
}

void RobotState::SetPosition(Vector2 pos)
{
	m_position = pos;

	m_isPositionSet = true;
}

void RobotState::SetOrientation(float orientation)
{
	m_orientation = orientation;
	
	m_isOrientationSet = true;
}