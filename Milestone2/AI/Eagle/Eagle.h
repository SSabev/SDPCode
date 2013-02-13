#if !defined(EAGLE_H)
#define EAGLE_H

#include "../MathTypes/Vector2.h"

#include <vector>

class Eagle
{
public:
	Eagle();
	
	Vector2 IdentifyTarget(Vector2 ourRobotPos, Vector2 enemyRobotPos, Vector2 ballPos);
};

#endif