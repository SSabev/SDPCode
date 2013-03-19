#include "RobotState.h"

RobotState::RobotState()
{
	m_isPositionSet = false;
	m_isOrientationSet = false;
	m_hasBall = false;
}

RobotState::RobotState(float posX, float posY, float orientation)
{
	m_position.Set(posX, posY);
	m_orientation = orientation;

	m_isPositionSet = true;
	m_isOrientationSet = true;
}

RobotState::RobotState(Vector2 pos, float orientation)
{
	m_position = pos;
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

void RobotState::SetHasBall(bool hasBall)
{
	m_hasBall = hasBall;
}

// OPERATOR OVERLOADS

bool operator ==(const RobotState& rs1, const RobotState& rs2)
{
	if ((rs1.Position() == rs2.Position()) && (rs1.Orientation() == rs2.Orientation()))
	{
		return true;
	}

	return false;
}
