#include "ForeseeUnitTests.h"

#include "../Foresee/Foresee.h"
#include "../MathTypes/Vector2.h"
#include <list>
#include <iostream>
#include <sstream>

ForeseeUnitTests::ForeseeUnitTests()
{
    TEST_ADD(ForeseeUnitTests::ForeseeExtrapolatePositionFromPoints);
    TEST_ADD(ForeseeUnitTests::ForeseeExtrapolatePositionFromPointsOutwithPitchBoundaries);
    TEST_ADD(ForeseeUnitTests::ForeseeExtrapolatePositionFromPointsOnPitchBoundaries);
    TEST_ADD(ForeseeUnitTests::ForeseeExtrapolateAngle);    
    TEST_ADD(ForeseeUnitTests::ForeseeExtrapolateAngleOver2PI);
    TEST_ADD(ForeseeUnitTests::ForeseeExtrapolateAngleBothZero);

}

void ForeseeUnitTests::ForeseeExtrapolatePositionFromPoints()
{
    /*std::vector<Vector2> positions;
    positions.push_back(Vector2(20,20));
	  positions.push_back(Vector2(10,10));

    Foresee foresee;
    foresee.SetPitchDimensions(244, 122);

    Vector2 extrapolatedPosition = foresee.ExtrapolatePositionFromPoints(positions);

    TEST_ASSERT(extrapolatedPosition == Vector2(30,30));*/

}

void ForeseeUnitTests::ForeseeExtrapolatePositionFromPointsOutwithPitchBoundaries()
{
    /*std::vector<Vector2> positions;
  	positions.push_back(Vector2(0,0));
  	positions.push_back(Vector2(20,20));

    Foresee foresee;
    foresee.SetPitchDimensions(244, 122);

    Vector2 extrapolatedPosition = foresee.ExtrapolatePositionFromPoints(positions);

    TEST_ASSERT(extrapolatedPosition == Vector2(0,0));*/

}

void ForeseeUnitTests::ForeseeExtrapolatePositionFromPointsOnPitchBoundaries()
{
    /*std::vector<Vector2> positions;
	  positions.push_back(Vector2(10,10));
  	positions.push_back(Vector2(20,20));

    Foresee foresee;
    foresee.SetPitchDimensions(244, 122);

    Vector2 extrapolatedPosition = foresee.ExtrapolatePositionFromPoints(positions);

    TEST_ASSERT(extrapolatedPosition == Vector2(0,0));*/

}

void ForeseeUnitTests::ForeseeExtrapolateAngle()
{
    /*std::vector<float> angles;
	angles.push_back(0.872664626f); // 50 degrees in radians
  	angles.push_back(0.820304748f); // 47 degrees in radians

    Foresee foresee;
    foresee.SetPitchDimensions(244, 122);

    float extrapolatedAngle = foresee.ExtrapolateAngle(angles);

    TEST_ASSERT(extrapolatedAngle == 0.925024504f);
    // Roughly 53 degrees. Calculation: 50 + (50-47)*/

}

void ForeseeUnitTests::ForeseeExtrapolateAngleOver2PI()
{
    /*std::vector<float> angles;
	angles.push_back(5.759586532f); // 330 degrees in radians
  	angles.push_back(-1.047197551f); // -60 degrees in radians

    Foresee foresee;
    foresee.SetPitchDimensions(244, 122);

    float extrapolatedAngle = foresee.ExtrapolateAngle(angles);  
    
    TEST_ASSERT(extrapolatedAngle == 12.56637f); 
    // Roughly 720 degrees. Calculation: 330 + (330 -(-60))*/

}

void ForeseeUnitTests::ForeseeExtrapolateAngleBothZero()
{
    /*std::vector<float> angles;
	angles.push_back(0.0f); // 0 degrees in radians
  	angles.push_back(0.0f); // 0 degrees in radians

    Foresee foresee;
    foresee.SetPitchDimensions(244, 122);

    float extrapolatedAngle = foresee.ExtrapolateAngle(angles);  

    TEST_ASSERT(extrapolatedAngle == 0.0f); 
    // 0 degrees*/

}