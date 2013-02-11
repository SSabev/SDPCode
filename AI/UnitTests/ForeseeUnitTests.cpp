#include "ForeseeUnitTests.h"

#include "../Foresee/Foresee.h"
#include "../MathTypes/Vector2.h"
#include <list>
#include <iostream>

ForeseeUnitTests::ForeseeUnitTests()
{
    TEST_ADD(ForeseeUnitTests::ForeseeExtrapolatePositionFromPoints);
    TEST_ADD(ForeseeUnitTests::ForeseeExtrapolatePositionFromPointsOutwithPitchBoundaries);
    TEST_ADD(ForeseeUnitTests::ForeseeExtrapolatePositionFromPointsOnPitchBoundaries);

}

void ForeseeUnitTests::ForeseeExtrapolatePositionFromPoints()
{
    std::vector<Vector2> positions;
    positions.push_back(Vector2(20,20));
	  positions.push_back(Vector2(10,10));

    Foresee foresee;
    foresee.SetPitchDimensions(244, 122);

    Vector2 extrapolatedPosition = foresee.ExtrapolatePositionFromPoints(positions);

    TEST_ASSERT(extrapolatedPosition == Vector2(30,30));

}

void ForeseeUnitTests::ForeseeExtrapolatePositionFromPointsOutwithPitchBoundaries()
{
    std::vector<Vector2> positions;
  	positions.push_back(Vector2(0,0));
  	positions.push_back(Vector2(20,20));

    Foresee foresee;
    foresee.SetPitchDimensions(244, 122);

    Vector2 extrapolatedPosition = foresee.ExtrapolatePositionFromPoints(positions);

    TEST_ASSERT(extrapolatedPosition == Vector2(0,0));

}

void ForeseeUnitTests::ForeseeExtrapolatePositionFromPointsOnPitchBoundaries()
{
    std::vector<Vector2> positions;
	  positions.push_back(Vector2(10,10));
  	positions.push_back(Vector2(20,20));

    Foresee foresee;
    foresee.SetPitchDimensions(244, 122);

    Vector2 extrapolatedPosition = foresee.ExtrapolatePositionFromPoints(positions);

    TEST_ASSERT(extrapolatedPosition == Vector2(0,0));

}
