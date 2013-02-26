#if !defined(FORESEEUNITTESTS_H)
#define FORESEEUNITTESTS_H

#include "cpptest.h"

class ForeseeUnitTests : public Test::Suite
{
public:
    ForeseeUnitTests();

private:
	
    void ForeseeExtrapolatePosition();
    void ForeseeExtrapolatePositionOutwithPitchBoundaries();
    void ForeseeExtrapolatePositionOnPitchBoundaries();
    void ForeseeExtrapolateRobotState();
    void ForeseeExtrapolateRobotStateOver2PI();
    void ForeseeExtrapolateRobotStateBothZero();

};

#endif
