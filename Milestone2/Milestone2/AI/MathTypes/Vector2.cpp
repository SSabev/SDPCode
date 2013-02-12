#include "Vector2.h"

#include <assert.h>
#include <math.h>
#include <float.h>

#include <sstream>

#define _USE_MATH_DEFINES

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

const float Vector2::X() const 
{
	assert(m_isSet);

	return m_x;
};
	
const float Vector2::Y() const 
{
	assert(m_isSet);

	return m_y;
};

// Restricts this vector to being within a rectangle, the two extreme 
// corners of which are defined by the min and max vectors.
void Vector2::Clamp(Vector2 minVector, Vector2 maxVector)
{
	if (X() < minVector.X())
	{
		m_x = minVector.X();
	}
	else if (X() > maxVector.X())
	{
		m_x = maxVector.X();
	}

	if (Y() < minVector.Y())
	{
		m_y = minVector.Y();
	}
	else if (Y() > maxVector.Y())
	{
		m_y = maxVector.Y();
	}
}

const float Vector2::Distance(Vector2* dest) const
{
	assert(m_isSet);

	float dist = sqrt(DistanceSquared(dest));

	return dist;
}

const float Vector2::DistanceSquared(Vector2* dest) const
{
	assert(m_isSet);

	// Note that these values are not absolute.
	float xDist = X() - dest->X();
	float yDist = Y() - dest->Y();

	float distSquared = pow(xDist,2) + pow(yDist,2);
	
	return distSquared;
}

const float Vector2::GetAngleTo(Vector2* dest) const
{
	assert(m_isSet);

	float xDist = dest->X() - X();
	float yDist = dest->Y() - Y();

	float angleRadians = atan2(yDist, xDist);

	return angleRadians;
}

const float Vector2::Gradient(Vector2* dest) const
{
	assert(m_isSet);

	const float xDifference = dest->X() - X();
	const float yDifference = dest->Y() - Y();

	float gradient;
	
	if (yDifference == 0)
	{
		gradient = 0;
	}
	else if (xDifference == 0)
	{
		gradient = FLT_MAX;
	}
	else
	{
		gradient = yDifference/xDifference;
	}

	return gradient;
}

const float Vector2::MagnitudeSquared() const
{
	assert(m_isSet);

	const float magSquared = pow(X(),2) + pow(Y(),2);

	return magSquared;
}

const std::string Vector2::ToString() const
{
	assert(m_isSet);

	std::stringstream ss;

	ss << X();
	ss << " ";
	ss << Y();
	
	std::string outputString = ss.str();

	return outputString;
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

Vector2 operator -(const Vector2& v1, const Vector2& v2)
{
	return Vector2(v1.X()-v2.X(), v1.Y()-v2.Y());
}

Vector2 operator *(const Vector2& v, const int& i)
{
	return Vector2(v.X()*i, v.Y()*i);
}

Vector2 operator *(const int& i, const Vector2& v)
{
	return v*i;
}

Vector2 operator *(const Vector2& v, const float& f)
{
	return Vector2(v.X()*f, v.Y()*f);
}

Vector2 operator *(const float& f, const Vector2& v)
{
	return v*f;
}

Vector2 operator /(const Vector2& v, const int& i)
{
	return Vector2(v.X()/i, v.Y()/i);
}

Vector2 operator /(const Vector2& v, const float& f)
{
	return Vector2(v.X()/f, v.Y()/f);
}

Vector2 Vector2::operator+=(const Vector2& v2)
{
	return *this+v2;
}

Vector2 Vector2::operator-=(const Vector2& v2)
{
	return *this-v2;
}

Vector2 Vector2::operator*=(const int& i)
{
	return *this * i;
}
Vector2 Vector2::operator*=(const float& f)
{
	return *this * f;
}

Vector2 Vector2::operator/=(const int& i)
{
	return *this / i;
}
Vector2 Vector2::operator/=(const float& f)
{
	return *this / f;
}
