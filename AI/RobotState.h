#if !defined(ROBOTSTATE_H)
#define ROBOTSTATE_H

#include "MathTypes/Vector2.h"

class RobotState
{
public:
	RobotState();
	RobotState(float posX, float posY, float orientation);
	RobotState(Vector2 pos, float orientation);

	const Vector2 Position() const
	{
		return m_position;
	};

	const float Orientation() const
	{
		return m_orientation;
	}
	
	const bool HasBall() const
	{
		return m_hasBall;
	}

	void SetPosition(float posX, float posY);
	void SetPosition(Vector2 pos);
	void SetOrientation(float orientation);
	void SetHasBall(bool hasBall);

private:
	bool m_isPositionSet;
	bool m_isOrientationSet;

	Vector2 m_position;
	float m_orientation;
	bool m_hasBall;
};

// Operator overloads.
bool operator==(const RobotState& rs1, const RobotState& rs2);

#endif
