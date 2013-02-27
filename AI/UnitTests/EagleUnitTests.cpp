#include "EagleUnitTests.h"

#include "../Eagle/Eagle.h"
#include "../MathTypes/Vector2.h"
#include <vector>

EagleUnitTests::EagleUnitTests()
{
	TEST_ADD(EagleUnitTests::EagleIdentifyTarget);
    TEST_ADD(EagleUnitTests::EagleIdentifyTargetBallOutOfRangeX);
	TEST_ADD(EagleUnitTests::EagleIdentifyTargetBallOutOfRangeY);
    TEST_ADD(EagleUnitTests::EagleIdentifyTargetBallOutOfRangeXAndY);
    TEST_ADD(EagleUnitTests::EagleDoWeHaveBall);
    TEST_ADD(EagleUnitTests::EagleDoWeHaveBallBoundaryPass);
    TEST_ADD(EagleUnitTests::EagleDoWeHaveBallBoundaryFail);
    TEST_ADD(EagleUnitTests::EagleDoWeHaveBallWrapTest);
}

void EagleUnitTests::EagleIdentifyTarget()
{
	Eagle eagle;

    eagle.SetPitchDimensions(244, 122);

    RobotState ourRobotState(Vector2(50, 70), 0.0f);
    RobotState enemyRobotState(Vector2(80, 30), 200.0f);
    Vector2 ball(20, 60);

	TEST_ASSERT(eagle.IdentifyTarget(ourRobotState, enemyRobotState, ball) == (RobotState(Vector2(20, 60),0.0f)));
}

void EagleUnitTests::EagleIdentifyTargetBallOutOfRangeX()
{
	Eagle eagle;

    eagle.SetPitchDimensions(244, 122);

    RobotState ourRobotState(Vector2(50, 70), 0.0f);
    RobotState enemyRobotState(Vector2(80, 30), 200.0f);
    Vector2 ball(-50, 87);

	TEST_ASSERT(eagle.IdentifyTarget(ourRobotState, enemyRobotState, ball) == (RobotState(Vector2(0, 87),0.0f)));
}

void EagleUnitTests::EagleIdentifyTargetBallOutOfRangeY()
{
	Eagle eagle;

    eagle.SetPitchDimensions(244, 122);

    RobotState ourRobotState(Vector2(50, 70), 0.0f);
    RobotState enemyRobotState(Vector2(80, 30), 200.0f);
    Vector2 ball(30, -100);

	TEST_ASSERT(eagle.IdentifyTarget(ourRobotState, enemyRobotState, ball) == (RobotState(Vector2(30, 0),0.0f)));
}

void EagleUnitTests::EagleIdentifyTargetBallOutOfRangeXAndY()
{
	Eagle eagle;

    eagle.SetPitchDimensions(244, 122);

    RobotState ourRobotState(Vector2(50, 70), 0.0f);
    RobotState enemyRobotState(Vector2(80, 30), 200.0f);
    Vector2 ball(600, -100);

	TEST_ASSERT(eagle.IdentifyTarget(ourRobotState, enemyRobotState, ball) == (RobotState(Vector2(243, 0),0.0f)));
}

void EagleUnitTests::EagleDoWeHaveBall()
{
	Eagle eagle;

    eagle.SetPitchDimensions(244, 122);

    RobotState ourRobotState(Vector2(10, 10), 1.0f);    
    Vector2 ball(50, 50);

	TEST_ASSERT(eagle.DoWeHaveBall(ourRobotState, ball) == true);
}

void EagleUnitTests::EagleDoWeHaveBallBoundaryPass()
{
	Eagle eagle;

    eagle.SetPitchDimensions(244, 122);

    RobotState ourRobotState(Vector2(10, 10), 1.56f/*1.5707963267948966f*/);    
    Vector2 ball(50, 50);

	TEST_ASSERT(eagle.DoWeHaveBall(ourRobotState, ball) == true);
}

void EagleUnitTests::EagleDoWeHaveBallBoundaryFail()
{
	Eagle eagle;

    eagle.SetPitchDimensions(244, 122);

    RobotState ourRobotState(Vector2(10, 10), 1.5707963267948966f);    
    Vector2 ball(50, 50);

	TEST_ASSERT(eagle.DoWeHaveBall(ourRobotState, ball) == false);
}

void EagleUnitTests::EagleDoWeHaveBallWrapTest()
{
	Eagle eagle;

    eagle.SetPitchDimensions(244, 122);

    RobotState ourRobotState(Vector2(10, 10), 7.283185307179586f);    
    Vector2 ball(50, 50);

	TEST_ASSERT(eagle.DoWeHaveBall(ourRobotState, ball) == true);
}
