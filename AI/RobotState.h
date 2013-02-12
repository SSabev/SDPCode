#if !defined(ROBOTSTATE_H)
#define ROBOTSTATE_H

#include "MathTypes/Vector2.h"

class RobotState
{
public:
	RobotState();
	RobotState(float posX, float posY, float orientation);
	RobotState(Vector2 pos, float orientation);

	Vector2 Position()
	{
		return m_position;
	};

	float Orientation()
	{
		return m_orientation;
	}

	void SetPosition(float posX, float posY);
	void SetPosition(Vector2 pos);
	void SetOrientation(float orientation);

private:
	bool m_isPositionSet;
	bool m_isOrientationSet;

	Vector2 m_position;
	float m_orientation;
};

#endif