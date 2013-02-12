#include "Vector2UnitTests.h"

#include "../MathTypes/Vector2.h"

Vector2UnitTests::Vector2UnitTests()
{
	TEST_ADD(Vector2UnitTests::Vector2Equals);
	TEST_ADD(Vector2UnitTests::Vector2DoesNotEqual);
	TEST_ADD(Vector2UnitTests::Vector2DoesNotEqual2);
	TEST_ADD(Vector2UnitTests::Vector2Addition);
	TEST_ADD(Vector2UnitTests::Vector2CompoundAddition);	
	TEST_ADD(Vector2UnitTests::Vector2Subtraction);
	TEST_ADD(Vector2UnitTests::Vector2CompoundSubtraction);
	TEST_ADD(Vector2UnitTests::Vector2MultiplicationInt);
	TEST_ADD(Vector2UnitTests::Vector2MultiplicationFloat);
	TEST_ADD(Vector2UnitTests::Vector2MultiplicationFloat2);
	TEST_ADD(Vector2UnitTests::Vector2CompoundMultiplicationInt);
	TEST_ADD(Vector2UnitTests::Vector2CompoundMultiplicationFloat);
	TEST_ADD(Vector2UnitTests::Vector2DivisionInt);
	TEST_ADD(Vector2UnitTests::Vector2DivisionFloat);
	TEST_ADD(Vector2UnitTests::Vector2CompoundDivisionInt);
	TEST_ADD(Vector2UnitTests::Vector2CompoundDivisionFloat);
	
	TEST_ADD(Vector2UnitTests::Vector2Distance);
	TEST_ADD(Vector2UnitTests::Vector2DistanceSquared);
	TEST_ADD(Vector2UnitTests::Vector2MagnitudeSquared);
	TEST_ADD(Vector2UnitTests::Vector2MagnitudeSquaredZero);
	TEST_ADD(Vector2UnitTests::Vector2ClampOutsideX);
        TEST_ADD(Vector2UnitTests::Vector2ClampInsideX);
        TEST_ADD(Vector2UnitTests::Vector2ClampOutsideY);
        TEST_ADD(Vector2UnitTests::Vector2ClampInsideY);
        TEST_ADD(Vector2UnitTests::Vector2ClampBothOnBoundaries);
        TEST_ADD(Vector2UnitTests::Vector2ClampBothOutwithBoundaries);
        TEST_ADD(Vector2UnitTests::Vector2GetAngleTo);
        TEST_ADD(Vector2UnitTests::Vector2GetAngleToNegative);
        TEST_ADD(Vector2UnitTests::Vector2GetAngleToZero);
        TEST_ADD(Vector2UnitTests::Vector2Gradient);
        TEST_ADD(Vector2UnitTests::Vector2GradientNegative);
        TEST_ADD(Vector2UnitTests::Vector2GradientZero);
        TEST_ADD(Vector2UnitTests::Vector2GradientDivideByZero);

}

void Vector2UnitTests::Vector2Equals()
{
	Vector2 v1(5,9);
	Vector2 v2(5,9);
	
	TEST_ASSERT(v1 == v2);
}

void Vector2UnitTests::Vector2DoesNotEqual()
{
	Vector2 v1(5,9);
	Vector2 v2(-3,9);
	
	TEST_ASSERT(v1 != v2);
}

void Vector2UnitTests::Vector2DoesNotEqual2()
{
	Vector2 v1(1,1);
	Vector2 v2(-1,-1);
	
	TEST_ASSERT(v1 != v2);
}

void Vector2UnitTests::Vector2Addition()
{
	Vector2 v1(5,9);
	Vector2 v2(-2,3);

	TEST_ASSERT((v1 + v2) == Vector2(3,12));
}

void Vector2UnitTests::Vector2CompoundAddition()
{
	Vector2 v1(5,9);
	Vector2 v2(-2,3);

	TEST_ASSERT((v1 += v2) == Vector2(3,12));
}

void Vector2UnitTests::Vector2Subtraction()
{
	Vector2 v1(5,9);
	Vector2 v2(-2,3);

	TEST_ASSERT((v1 - v2) == Vector2(7,6));
}

void Vector2UnitTests::Vector2CompoundSubtraction()
{
	Vector2 v1(5,9);
	Vector2 v2(-2,3);

	TEST_ASSERT((v1 -= v2) == Vector2(7,6));
}

void Vector2UnitTests::Vector2MultiplicationInt()
{
	Vector2 v(5,9);
	int i = 3;

	TEST_ASSERT((i * v) == Vector2(15,27));
}

void Vector2UnitTests::Vector2MultiplicationFloat()
{
	Vector2 v(5,9);
	float f = 1.5f;

	TEST_ASSERT((f * v) == Vector2(7.5f,13.5f));
}

void Vector2UnitTests::Vector2MultiplicationFloat2()
{
	Vector2 v(5,9);
	float f = 0.0f;

	TEST_ASSERT((f * v) == Vector2(0.0f,0.0f));
}


void Vector2UnitTests::Vector2CompoundMultiplicationInt()
{
	Vector2 v(5,9);
	int i = 3;

	TEST_ASSERT((v *= i) == Vector2(15,27));
}

void Vector2UnitTests::Vector2CompoundMultiplicationFloat()
{
	Vector2 v(5,9);
	float f = 1.5f;

	TEST_ASSERT((v *= f) == Vector2(7.5f,13.5f));
}

void Vector2UnitTests::Vector2DivisionInt()
{
	Vector2 v(5,9);
	int i = 2;

	TEST_ASSERT((v/i) == Vector2(2.5f,4.5f));
}

void Vector2UnitTests::Vector2DivisionFloat()
{
	Vector2 v(5,9);
	float f = 0.5f;

	TEST_ASSERT((v/f) == Vector2(10,18));
}

void Vector2UnitTests::Vector2CompoundDivisionInt()
{
	Vector2 v(5,9);
	int i = 2;

	TEST_ASSERT((v /= i) == Vector2(2.5f,4.5f));
}

void Vector2UnitTests::Vector2CompoundDivisionFloat()
{
	Vector2 v(5,9);
	float f = 0.5f;

	TEST_ASSERT((v /= f) == Vector2(10,18));
}

void Vector2UnitTests::Vector2Distance()
{
	Vector2 v1(3,-10);
	Vector2 v2(0,-6);

	TEST_ASSERT(v1.Distance(&v2) == 5);
}

void Vector2UnitTests::Vector2DistanceSquared()
{
	Vector2 v1(3,-2);
	Vector2 v2(3,1);

	TEST_ASSERT(v1.DistanceSquared(&v2) == 9);
}

void Vector2UnitTests::Vector2MagnitudeSquared()
{
	Vector2 v1(-5,0);

	TEST_ASSERT(v1.MagnitudeSquared() == 25);
}

void Vector2UnitTests::Vector2MagnitudeSquaredZero()
{
	Vector2 v1(0,0);

	TEST_ASSERT(v1.MagnitudeSquared() == 0);
}

void Vector2UnitTests::Vector2ClampOutsideX()
{
    Vector2 v1 (6, 12);
    Vector2 v2 (8, 8);
    Vector2 v3 (15, 15);
    Vector2 v4 (8, 12);

    v1.Clamp(v2,v3);

    TEST_ASSERT(v1 == v4);

}

void Vector2UnitTests::Vector2ClampInsideX()
{
    Vector2 v1 (9, 12);
    Vector2 v2 (8, 8);
    Vector2 v3 (15, 15);    

    v1.Clamp(v2,v3);

    TEST_ASSERT(v1 == Vector2(9,12));

}

void Vector2UnitTests::Vector2ClampOutsideY()
{
    Vector2 v1 (9, 17);
    Vector2 v2 (8, 8);
    Vector2 v3 (15, 15);
    Vector2 v4 (9, 15);

    v1.Clamp(v2,v3);

    TEST_ASSERT(v1 == v4);

}

void Vector2UnitTests::Vector2ClampInsideY()
{
    Vector2 v1 (9, 12);
    Vector2 v2 (8, 8);
    Vector2 v3 (15, 15);    

    v1.Clamp(v2,v3);

    TEST_ASSERT(v1 == Vector2(9,12));

}

void Vector2UnitTests::Vector2ClampBothOnBoundaries()
{
    Vector2 v1 (8, 15);
    Vector2 v2 (8, 8);
    Vector2 v3 (15, 15);    

    v1.Clamp(v2,v3);

    TEST_ASSERT(v1 == Vector2(8,15));

}

void Vector2UnitTests::Vector2ClampBothOutwithBoundaries()
{
    Vector2 v1 (7, 16);
    Vector2 v2 (8, 8);
    Vector2 v3 (15, 15);    

    v1.Clamp(v2,v3);

    TEST_ASSERT(v1 == Vector2(8,15));

}

void Vector2UnitTests::Vector2GetAngleTo()
{
    Vector2 v1 (10, 10);        
    Vector2 v2 (20, 20);    

    TEST_ASSERT(v1.GetAngleTo(&v2) == 0.7853981633974483f);

}

void Vector2UnitTests::Vector2GetAngleToNegative()
{
    Vector2 v1 (20, 20);        
    Vector2 v2 (10, 10);    

    TEST_ASSERT(v1.GetAngleTo(&v2) == -2.356194490192345f);

}

void Vector2UnitTests::Vector2GetAngleToZero()
{
    Vector2 v1 (20, 20);        
    Vector2 v2 (20, 20);    

    TEST_ASSERT(v1.GetAngleTo(&v2) == 0.0f);

}


void Vector2UnitTests::Vector2Gradient()
{
    Vector2 v1 (5, 5);        
    Vector2 v2 (10, 10);    

    TEST_ASSERT(v1.Gradient(&v2) == 1.0f);

}

void Vector2UnitTests::Vector2GradientNegative()
{
    Vector2 v1 (5, 10);        
    Vector2 v2 (10, 6);    

    TEST_ASSERT(v1.Gradient(&v2) == -0.8f);

}

void Vector2UnitTests::Vector2GradientZero()
{
    Vector2 v1 (2, 10);        
    Vector2 v2 (5, 10);    

    TEST_ASSERT(v1.Gradient(&v2) == 0.0f);

}

void Vector2UnitTests::Vector2GradientDivideByZero()
{
    Vector2 v1 (5, 1);        
    Vector2 v2 (5, 12);      

    TEST_ASSERT(v1.Gradient(&v2) == 340282346638528859811704183484516925440.000000f);

}

