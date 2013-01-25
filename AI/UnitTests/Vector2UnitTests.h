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
	void Vector2Addition();
	void Vector2Subtraction();
	
	void Vector2Distance();
	void Vector2DistanceSquared();
	void Vector2MagnitudeSquared();
};

#endif