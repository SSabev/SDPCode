#include "RobotState.h"

RobotState::RobotState(float posX, float posY, float orientation)
{
	m_position.Set(posX, posY);
	m_orientation = orientation;
}