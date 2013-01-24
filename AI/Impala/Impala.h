#if !defined(IMPALA_H)
#define IMPALA_H

#include "MathTypes/Vector2.h"

#include <vector>

class Impala
{
public:
	Impala();
	
	std::vector<Vector2> SmoothPath(std::vector<Vector2> aStarPath);
};

#endif