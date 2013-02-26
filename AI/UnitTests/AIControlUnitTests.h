#if !defined(AICONTROLUNITTESTS_H)
#define AICONTROLUNITTESTS_H

#include "cpptest.h"

class AIControlUnitTests : public Test::Suite
{
public:
  AIControlUnitTests();

private:
    void AIControlCoordinatesAreBad();
    void AIControlCoordinatesAreBadBoundaries();
    void AIControlCoordinatesAreBadNegativeX();
    void AIControlCoordinatesAreBadNegativeY();
    void AIControlCoordinatesAreBadBothNegative();
    void AIControlCoordinatesAreBadOversizedX();
    void AIControlCoordinatesAreBadOversizedY();
    void AIControlCoordinatesAreBadBothOversized();
    void AIControlIsFailedFrame();
    void AIControlIsFailedFrameBoundaries();
    void AIControlIsFailedFrameRobot1();
    void AIControlIsFailedFrameBothRobots();
    void AIControlIsFailedFrameAllObjects();
};




#endif
