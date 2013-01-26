#include "Vector2UnitTests.h"

#include "../MathTypes/Vector2.h"

Vector2UnitTests::Vector2UnitTests()
{
	TEST_ADD(Vector2UnitTests::Vector2Equals);
	TEST_ADD(Vector2UnitTests::Vector2DoesNotEqual);
	TEST_ADD(Vector2UnitTests::Vector2Addition);
	TEST_ADD(Vector2UnitTests::Vector2CompoundAddition);	
	TEST_ADD(Vector2UnitTests::Vector2Subtraction);
	TEST_ADD(Vector2UnitTests::Vector2CompoundSubtraction);
	TEST_ADD(Vector2UnitTests::Vector2MultiplicationInt);
	TEST_ADD(Vector2UnitTests::Vector2MultiplicationFloat);
	TEST_ADD(Vector2UnitTests::Vector2CompoundMultiplicationInt);
	TEST_ADD(Vector2UnitTests::Vector2CompoundMultiplicationFloat);
	TEST_ADD(Vector2UnitTests::Vector2DivisionInt);
	TEST_ADD(Vector2UnitTests::Vector2DivisionFloat);
	TEST_ADD(Vector2UnitTests::Vector2CompoundDivisionInt);
	TEST_ADD(Vector2UnitTests::Vector2CompoundDivisionFloat);
	
	TEST_ADD(Vector2UnitTests::Vector2Distance);
	TEST_ADD(Vector2UnitTests::Vector2DistanceSquared);
	TEST_ADD(Vector2UnitTests::Vector2MagnitudeSquared);
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