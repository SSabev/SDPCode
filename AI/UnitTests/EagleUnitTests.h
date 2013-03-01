#if !defined(EAGLEUNITTESTS_H)
#define EAGLEUNITTESTS_H

#include "cpptest.h"
#include "../MathTypes/Vector2.h"
#include <vector>

class EagleUnitTests : public Test::Suite
{
public:
    EagleUnitTests();

private:
    void EagleIdentifyTarget();
    void EagleIdentifyTargetBallOutOfRangeX();
    void EagleIdentifyTargetBallOutOfRangeY();
    void EagleIdentifyTargetBallOutOfRangeXAndY();
    void EagleDoWeHaveBall();
    void EagleDoWeHaveBall_OrientationBoundaryPass();
    void EagleDoWeHaveBall_OrientationBoundaryFail();
    void EagleDoWeHaveBall_OrientationWrapTest();
    void EagleDoWeHaveBall_DistanceBoundaryPass();
    void EagleDoWeHaveBall_DistanceBoundaryFail();
    void EagleDoWeHaveBall_BothFail();
};
#endif
