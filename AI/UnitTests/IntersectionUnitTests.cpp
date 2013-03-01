#include "IntersectionUnitTests.h"

#include "../MathTypes/Vector2.h"
#include "../MathTypes/Intersection.h"

IntersectionUnitTests::IntersectionUnitTests()
{
    TEST_ADD(IntersectionUnitTests::IntersectionTest);
    TEST_ADD(IntersectionUnitTests::IntersectionTestFail);
    TEST_ADD(IntersectionUnitTests::IntersectionTestBoundaryPass);
}


/*              
                                    -   Think of line as a "ray" - continuing on forever after the line ends (and also before it begins).
                                -       Like an equation of a line, infinite length.
                            -
                        -
                    -
                lineEnd
              -   
        lineStart
       -
    -
 -
*/


void IntersectionUnitTests::IntersectionTest()
{
    Intersection intersection;

    Vector2 lineStart(5,10);
	  Vector2 lineEnd(2,4);
    Vector2 circleCentre(3,6);

    float circleRadius = 2.0f;
	
	  TEST_ASSERT(intersection.LineCircleIntersection(lineStart,lineEnd,circleCentre, circleRadius) == true);
}

void IntersectionUnitTests::IntersectionTestFail()
{
    Intersection intersection;

	  Vector2 lineStart(1,1);
	  Vector2 lineEnd(2,2);
    Vector2 circleCentre(400, 200);

    float circleRadius = 1.0f;
	
	  TEST_ASSERT(intersection.LineCircleIntersection(lineStart,lineEnd,circleCentre, circleRadius) == false);
}

void IntersectionUnitTests::IntersectionTestBoundaryPass()
{
    Intersection intersection;

	  Vector2 lineStart(3,0);
	  Vector2 lineEnd(3,4);
    Vector2 circleCentre(2, 2);

    float circleRadius = 1.0f;
	
	  TEST_ASSERT(intersection.LineCircleIntersection(lineStart,lineEnd,circleCentre, circleRadius) == true);
}

