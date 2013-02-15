#if !defined(FORESEEUNITTESTS_H)
#define FORESEEUNITTESTS_H

#include "cpptest.h"

class ForeseeUnitTests : public Test::Suite
{
public:
    ForeseeUnitTests();

private:
	
    void ForeseeExtrapolatePositionFromPoints();
    void ForeseeExtrapolatePositionFromPointsOutwithPitchBoundaries();
    void ForeseeExtrapolatePositionFromPointsOnPitchBoundaries();
    void ForeseeExtrapolateAngle();
    void ForeseeExtrapolateAngleOver2PI();
    void ForeseeExtrapolateAngleBothZero();

};

#endif
