#if !defined(EAGLE_H)
#define EAGLE_H

#include "MathTypes/Vector2.h"

#include <vector>

class Eagle
{
public:
	Eagle();
	
	Vector2 IdentifyTarget(std::vector<Vector2> positions);
};

#endif