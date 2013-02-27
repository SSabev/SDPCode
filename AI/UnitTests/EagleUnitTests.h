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
};
#endif
