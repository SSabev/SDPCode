#include "AIControlUnitTests.h"

#include <vector>

#include "../AIControl.h"
#include "../../Shared/SharedMem.h"
#include "../MathTypes/Vector2.h"

#include <string.h>
#include <iostream>
#include <fstream>
#include <algorithm>

AIControlUnitTests::AIControlUnitTests()
{
    TEST_ADD(AIControlUnitTests::AIControlCoordinatesAreBad);
    TEST_ADD(AIControlUnitTests::AIControlCoordinatesAreBadBoundaries);
    TEST_ADD(AIControlUnitTests::AIControlCoordinatesAreBadNegativeX);
    TEST_ADD(AIControlUnitTests::AIControlCoordinatesAreBadNegativeY);
    TEST_ADD(AIControlUnitTests::AIControlCoordinatesAreBadBothNegative);
    TEST_ADD(AIControlUnitTests::AIControlCoordinatesAreBadOversizedX);
    TEST_ADD(AIControlUnitTests::AIControlCoordinatesAreBadOversizedY);
    TEST_ADD(AIControlUnitTests::AIControlCoordinatesAreBadBothOversized);
    TEST_ADD(AIControlUnitTests::AIControlIsFailedFrame);
    TEST_ADD(AIControlUnitTests::AIControlIsFailedFrameBoundaries);
    TEST_ADD(AIControlUnitTests::AIControlIsFailedFrameRobot1);
    TEST_ADD(AIControlUnitTests::AIControlIsFailedFrameBothRobots);
    TEST_ADD(AIControlUnitTests::AIControlIsFailedFrameAllObjects);
}


void AIControlUnitTests::AIControlCoordinatesAreBad()
{
    AIControl aiControl;
    TShMem sharedMem;

    sharedMem.pitchCfg.pitchWidth = 600;
    sharedMem.pitchCfg.pitchHeight = 300;

    aiControl.Initialise(&sharedMem);

    Vector2 objectPosition(10,10);
    

    TEST_ASSERT(aiControl.CoordinatesAreBad(objectPosition) == false);


}

void AIControlUnitTests::AIControlCoordinatesAreBadBoundaries()
{
    AIControl aiControl;
    TShMem sharedMem;

    sharedMem.pitchCfg.pitchWidth = 600;
    sharedMem.pitchCfg.pitchHeight = 300;

    aiControl.Initialise(&sharedMem);

    Vector2 objectPosition(0,299);
    

    TEST_ASSERT(aiControl.CoordinatesAreBad(objectPosition) == false);


}

void AIControlUnitTests::AIControlCoordinatesAreBadNegativeX()
{
    AIControl aiControl;
    TShMem sharedMem;

    sharedMem.pitchCfg.pitchWidth = 600;
    sharedMem.pitchCfg.pitchHeight = 300;

    aiControl.Initialise(&sharedMem);

    Vector2 objectPosition(-10,10);
    

    TEST_ASSERT(aiControl.CoordinatesAreBad(objectPosition) == true);


}

void AIControlUnitTests::AIControlCoordinatesAreBadNegativeY()
{
    AIControl aiControl;
    TShMem sharedMem;

    sharedMem.pitchCfg.pitchWidth = 600;
    sharedMem.pitchCfg.pitchHeight = 300;

    aiControl.Initialise(&sharedMem);

    Vector2 objectPosition(10,-10);
    

    TEST_ASSERT(aiControl.CoordinatesAreBad(objectPosition) == true);


}


void AIControlUnitTests::AIControlCoordinatesAreBadBothNegative()
{
    AIControl aiControl;
    TShMem sharedMem;

    sharedMem.pitchCfg.pitchWidth = 600;
    sharedMem.pitchCfg.pitchHeight = 300;

    aiControl.Initialise(&sharedMem);

    Vector2 objectPosition(-10,-10);
    

    TEST_ASSERT(aiControl.CoordinatesAreBad(objectPosition) == true);


}

void AIControlUnitTests::AIControlCoordinatesAreBadOversizedX()
{
    AIControl aiControl;
    TShMem sharedMem;

    sharedMem.pitchCfg.pitchWidth = 600;
    sharedMem.pitchCfg.pitchHeight = 300;

    aiControl.Initialise(&sharedMem);

    Vector2 objectPosition(700,200);
    

    TEST_ASSERT(aiControl.CoordinatesAreBad(objectPosition) == true);


}

void AIControlUnitTests::AIControlCoordinatesAreBadOversizedY()
{
    AIControl aiControl;
    TShMem sharedMem;

    sharedMem.pitchCfg.pitchWidth = 600;
    sharedMem.pitchCfg.pitchHeight = 300;

    aiControl.Initialise(&sharedMem);

    Vector2 objectPosition(500,500);
    

    TEST_ASSERT(aiControl.CoordinatesAreBad(objectPosition) == true);


}

void AIControlUnitTests::AIControlCoordinatesAreBadBothOversized()
{
    AIControl aiControl;
    TShMem sharedMem;

    sharedMem.pitchCfg.pitchWidth = 600;
    sharedMem.pitchCfg.pitchHeight = 300;

    aiControl.Initialise(&sharedMem);

    Vector2 objectPosition(700,500);
    

    TEST_ASSERT(aiControl.CoordinatesAreBad(objectPosition) == true);


}

void AIControlUnitTests::AIControlIsFailedFrame()
{
    AIControl aiControl;
    TShMem sharedMem;

    sharedMem.pitchCfg.pitchWidth = 600;
    sharedMem.pitchCfg.pitchHeight = 300;

    aiControl.Initialise(&sharedMem);

    RobotState robot1Position(Vector2(200,200),0);
    RobotState robot2Position(Vector2(500,100),0);
    Vector2 ballPosition(300,50); 
    

    TEST_ASSERT(aiControl.IsFailedFrame(robot1Position) == false);

}

void AIControlUnitTests::AIControlIsFailedFrameBoundaries()
{
    AIControl aiControl;
    TShMem sharedMem;

    sharedMem.pitchCfg.pitchWidth = 600;
    sharedMem.pitchCfg.pitchHeight = 300;

    aiControl.Initialise(&sharedMem);

    RobotState robot1Position(Vector2(599,200),0);
    RobotState robot2Position(Vector2(500,299),0);
    Vector2 ballPosition(599,299); 
    

    TEST_ASSERT(aiControl.IsFailedFrame(robot1Position) == false);

}

void AIControlUnitTests::AIControlIsFailedFrameRobot1()
{
    AIControl aiControl;
    TShMem sharedMem;

    sharedMem.pitchCfg.pitchWidth = 600;
    sharedMem.pitchCfg.pitchHeight = 300;

    aiControl.Initialise(&sharedMem);

    RobotState robot1Position(Vector2(1234,200),0);
    RobotState robot2Position(Vector2(500,100),0);
    Vector2 ballPosition(300,50); 
    

    TEST_ASSERT(aiControl.IsFailedFrame(robot1Position) == true);

}

void AIControlUnitTests::AIControlIsFailedFrameBothRobots()
{
    AIControl aiControl;
    TShMem sharedMem;

    sharedMem.pitchCfg.pitchWidth = 600;
    sharedMem.pitchCfg.pitchHeight = 300;

    aiControl.Initialise(&sharedMem);

    RobotState robot1Position(Vector2(1234,200),0);
    RobotState robot2Position(Vector2(17659756,1000),0);
    Vector2 ballPosition(300,50); 
    

    TEST_ASSERT(aiControl.IsFailedFrame(robot1Position) == true);

}

void AIControlUnitTests::AIControlIsFailedFrameAllObjects()
{
    AIControl aiControl;
    TShMem sharedMem;

    sharedMem.pitchCfg.pitchWidth = 600;
    sharedMem.pitchCfg.pitchHeight = 300;

    aiControl.Initialise(&sharedMem);

    RobotState robot1Position(Vector2(1234,200),0);
    RobotState robot2Position(Vector2(17659756,1000),0);
    Vector2 ballPosition(1300,-50); 
    

    TEST_ASSERT(aiControl.IsFailedFrame(robot1Position) == true);

}
