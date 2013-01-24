#include "Vector2.h"

#include <assert.h>
#include <math.h>

Vector2::Vector2()
{
	m_isSet = false;
}

Vector2::Vector2(float x, float y)
{
	Set(x,y);
}

void Vector2::Set(float x, float y)
{
	m_x = x;
	m_y = y;

	m_isSet = true;
}

float Vector2::X() const 
{
	assert(m_isSet);

	return m_x;
};
	
float Vector2::Y() const 
{
	assert(m_isSet);

	return m_y;
};

float Vector2::Distance(Vector2* dest) const
{
	assert(m_isSet);

	float dist = sqrt(DistanceSquared(dest));

	return dist;
}

float Vector2::DistanceSquared(Vector2* dest) const
{
	assert(m_isSet);

	// Note that these values are not absolute.
	float xDist = X() - dest->X();
	float yDist = Y() - dest->Y();

	float distSquared = pow(xDist,2) + pow(yDist,2);
	
	return distSquared;
}

float Vector2::MagnitudeSquared() const
{
	assert(m_isSet);

	const float magSquared = pow(X(),2) + pow(Y(),2);

	return magSquared;
}

// OPERATOR OVERLOADS

bool operator ==(const Vector2& v1, const Vector2& v2)
{
	if ((v1.X() == v2.X()) && (v1.Y() == v2.Y()))
	{
		return true;
	}

	return false;
}

bool operator !=(const Vector2& v1, const Vector2& v2)
{
	return !(v1==v2);
}

Vector2 operator +(const Vector2& v1, const Vector2& v2)
{
	return Vector2(v1.X()+v2.X(), v1.Y()+v2.Y());
}

Vector2 operator +=(const Vector2& v1, const Vector2& v2)
{
	return v1+v2;
}

Vector2 operator -(const Vector2& v1, const Vector2& v2)
{
	return Vector2(v1.X()-v2.X(), v1.Y()-v2.Y());
}

Vector2 operator -=(const Vector2& v1, const Vector2& v2)
{
	return v1-v2;
}