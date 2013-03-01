#if !defined(INTERSECTION_H)
#define INTERSECTION_H

#include "Vector2.h"

class Intersection
{
public:
	Intersection();

	bool LineCircleIntersection(Vector2 lineStart, Vector2 lineEnd, Vector2 circleCentre, float circleRadius);
};

#endif