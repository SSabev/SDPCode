#include "RobotState.h"

RobotState::RobotState()
{
	m_isSet = false;
}

RobotState::RobotState(float posX, float posY, float orientation)
{
	m_position.Set(posX, posY);
	m_orientation = orientation;

	m_isSet = true;
}