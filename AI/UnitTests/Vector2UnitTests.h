#if !defined(VECTOR2UNITTESTS_H)
#define VECTOR2UNITTESTS_H

#include "cpptest.h"

class Vector2UnitTests : public Test::Suite
{
public:
	Vector2UnitTests();

private:
	void Vector2Equals();
	void Vector2DoesNotEqual();
	void Vector2DoesNotEqual2();
	void Vector2Addition();
	void Vector2CompoundAddition();
	void Vector2Subtraction();
	void Vector2CompoundSubtraction();
	void Vector2MultiplicationInt();
	void Vector2MultiplicationFloat();
	void Vector2MultiplicationFloat2();
	void Vector2CompoundMultiplicationInt();
	void Vector2CompoundMultiplicationFloat();
	void Vector2DivisionInt();
	void Vector2DivisionFloat();
	void Vector2CompoundDivisionInt();
	void Vector2CompoundDivisionFloat();
	
	void Vector2Distance();
	void Vector2DistanceSquared();
	void Vector2MagnitudeSquared();
	void Vector2MagnitudeSquaredZero();
    
    	void Vector2ClampOutsideX();
    	void Vector2ClampInsideX();
    	void Vector2ClampOutsideY();
    	void Vector2ClampInsideY();
    	void Vector2ClampBothOnBoundaries();
    	void Vector2ClampBothOutwithBoundaries();
    	void Vector2GetAngleTo();
    	void Vector2GetAngleToNegative();
    	void Vector2GetAngleToZero();
    	void Vector2Gradient();
    	void Vector2GradientNegative();
    	void Vector2GradientZero();
    	void Vector2GradientDivideByZero();
    	void Vector2Dot();
    	void Vector2DotNegativeValuesPositiveOutcome();
    	void Vector2DotNegativeValuesNegativeOutcome();
    	void Vector2DotZero();
    	void Vector2OneNegative();                      
};

#endif
