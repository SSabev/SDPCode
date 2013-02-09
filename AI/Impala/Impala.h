#if !defined(IMPALA_H)
#define IMPALA_H

#include "../MathTypes/Vector2.h"

#include <list>
#include <vector>

class Impala
{
public:
	Impala();
	
	std::list<Vector2> SmoothPath(std::list<Vector2> aStarPath, int numAdditionalPasses=0);

private:
	std::list<Vector2> RunPass(std::list<Vector2> path);
	std::list<Vector2> CollapsePoints(std::list<Vector2> path);
};

#endif