#if !defined(ROBOTSTATE_H)
#define ROBOTSTATE_H

#include "MathTypes/Vector2.h"

class RobotState
{
public:
	RobotState(float posX, float posY, float orientation);

	Vector2 Position()
	{
		return m_position;
	};

	float Orientation()
	{
		return m_orientation;
	}

private:
	Vector2 m_position;
	float m_orientation;
};

#endif