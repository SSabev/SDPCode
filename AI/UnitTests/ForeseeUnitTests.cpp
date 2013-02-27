#include "ForeseeUnitTests.h"

#include "../Foresee/Foresee.h"
#include "../MathTypes/Vector2.h"
#include <list>
#include <iostream>
#include <sstream>

ForeseeUnitTests::ForeseeUnitTests()
{
    TEST_ADD(ForeseeUnitTests::ForeseeExtrapolatePosition);
    TEST_ADD(ForeseeUnitTests::ForeseeExtrapolatePositionOutwithPitchBoundaries);
    TEST_ADD(ForeseeUnitTests::ForeseeExtrapolatePositionOnPitchBoundaries);
    TEST_ADD(ForeseeUnitTests::ForeseeExtrapolateRobotState);    
    TEST_ADD(ForeseeUnitTests::ForeseeExtrapolateRobotStateOver2PI);
    TEST_ADD(ForeseeUnitTests::ForeseeExtrapolateRobotStateBothZero);

}

void ForeseeUnitTests::ForeseeExtrapolatePosition()
{
    std::vector<Vector2> positions;
    positions.push_back(Vector2(20,20));
	  positions.push_back(Vector2(10,10));

    Foresee foresee;
    foresee.SetPitchDimensions(244, 122);

    Vector2 extrapolatedPosition = foresee.ExtrapolatePosition(positions);

    TEST_ASSERT(extrapolatedPosition == Vector2(30,30));

}

void ForeseeUnitTests::ForeseeExtrapolatePositionOutwithPitchBoundaries()
{
    std::vector<Vector2> positions;
  	positions.push_back(Vector2(0,0));
  	positions.push_back(Vector2(20,20));

    Foresee foresee;
    foresee.SetPitchDimensions(244, 122);

    Vector2 extrapolatedPosition = foresee.ExtrapolatePosition(positions);

    TEST_ASSERT(extrapolatedPosition == Vector2(0,0));

}

void ForeseeUnitTests::ForeseeExtrapolatePositionOnPitchBoundaries()
{
    std::vector<Vector2> positions;
	positions.push_back(Vector2(10,10));
  	positions.push_back(Vector2(20,20));

    Foresee foresee;
    foresee.SetPitchDimensions(244, 122);

    Vector2 extrapolatedPosition = foresee.ExtrapolatePosition(positions);

    TEST_ASSERT(extrapolatedPosition == Vector2(0,0));

}

void ForeseeUnitTests::ForeseeExtrapolateRobotState()
{
    std::vector<RobotState> positions;
	positions.push_back(RobotState(Vector2(30,10),0.872664626f)); // 50 degrees in radians
  	positions.push_back(RobotState(Vector2(20,10),0.820304748f)); // 47 degrees in radians

    Foresee foresee;
    foresee.SetPitchDimensions(244, 122);

    RobotState extrapolatedAngle = foresee.ExtrapolateRobotState(positions);

    TEST_ASSERT(extrapolatedAngle == RobotState(Vector2(40,10), 0.925024504f));
    // Roughly 53 degrees. Calculation: 50 + (50-47)

}

void ForeseeUnitTests::ForeseeExtrapolateRobotStateOver2PI()
{
    std::vector<RobotState> angles;
	angles.push_back(RobotState(Vector2(30,10),5.759586532f)); // 330 degrees in radians
  	angles.push_back(RobotState(Vector2(20,10),-1.047197551f)); // -60 degrees in radians

    Foresee foresee;
    foresee.SetPitchDimensions(244, 122);

    RobotState extrapolatedAngle = foresee.ExtrapolateRobotState(angles);  
    
    TEST_ASSERT(extrapolatedAngle == RobotState(Vector2(40,10), 12.56637f)); 
    // Roughly 720 degrees. Calculation: 330 + (330 -(-60))

}

void ForeseeUnitTests::ForeseeExtrapolateRobotStateBothZero()
{
    std::vector<RobotState> angles;
	angles.push_back(RobotState(Vector2(30,10), 0.0f)); // 0 degrees in radians
  	angles.push_back(RobotState(Vector2(20,10), 0.0f)); // 0 degrees in radians

    Foresee foresee;
    foresee.SetPitchDimensions(244, 122);

    RobotState extrapolatedAngle = foresee.ExtrapolateRobotState(angles);  

    TEST_ASSERT(extrapolatedAngle == RobotState(Vector2(40,10), 0.0f)); 
    // 0 degrees

}
