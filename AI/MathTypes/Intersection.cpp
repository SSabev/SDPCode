#include "Intersection.h"

#include <cmath>

Intersection::Intersection()
{

}

bool Intersection::LineCircleIntersection(Vector2 lineStart, Vector2 lineEnd, Vector2 circleCentre, float circleRadius)
{
	// As posted here: http://stackoverflow.com/questions/1073336/circle-line-collision-detection
	Vector2 directionOfLine = lineEnd - lineStart;
	Vector2 directionCentreToLineStart = lineStart - circleCentre;
	
	float a = directionOfLine.Dot(&directionOfLine);
	float b = 2*directionCentreToLineStart.Dot(&directionOfLine);
	float c = directionCentreToLineStart.Dot(&directionCentreToLineStart) - std::pow(circleRadius,2);

	float discriminant = std::pow(b,2) - (4*a) + c;

	if (discriminant >= 0)
	{
		return true;
	}

	return false;
}