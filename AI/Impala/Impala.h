#if !defined(IMPALA_H)
#define IMPALA_H

#include "../MathTypes/Vector2.h"

#include <list>

class Impala
{
public:
	Impala();
	
	std::list<Vector2> SmoothPath(std::list<Vector2> aStarPath);
};

#endif