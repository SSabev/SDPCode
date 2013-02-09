#if !defined(VECTOR2_H)
#define VECTOR2_H

#include <string>

/* Vector2 represents a two-dimensional vector. 
 * This is useful for positions of entities.
 */
class Vector2
{
public:
	Vector2();
	Vector2(float x, float y);
	void Set(float x, float y);
	
	const float X() const;
	const float Y() const;

	void Clamp(Vector2 minVector, Vector2 maxVector);
	const float Distance(Vector2* dest) const;
	const float DistanceSquared(Vector2* dest) const;
	const float Gradient(Vector2* dest) const;
	const float MagnitudeSquared() const;

	const std::string ToString() const;

	// Operator overloads.
	Vector2 operator+=(const Vector2& v2);
	Vector2 operator-=(const Vector2& v2);
	Vector2 operator*=(const int& i);
	Vector2 operator*=(const float& f);
	Vector2 operator/=(const int& i);
	Vector2 operator/=(const float& f);
	
private:
	float m_x;
	float m_y;
	bool m_isSet;
};

// Operator overloads.
bool operator==(const Vector2& v1, const Vector2& v2);
bool operator!=(const Vector2& v1, const Vector2& v2);
Vector2 operator+(const Vector2& v1, const Vector2& v2);
Vector2 operator-(const Vector2& v1, const Vector2& v2);
Vector2 operator*(const int& i, const Vector2& v);
Vector2 operator*(const Vector2& v, const int& i);
Vector2 operator*(const float& f, const Vector2& v);
Vector2 operator*(const Vector2& v, const float& f);
Vector2 operator/(const Vector2& v, const int& i);
Vector2 operator/(const Vector2& v, const float& f);

// This is used as the comparer in maps.
struct Vector2Comparer 
{
    bool operator() (const Vector2 &v1, const Vector2 &v2) const 
	{
		// We have a problem using the vectors' magnitude here in that:
		// "Two keys are considered equivalent if the container's comparison object returns false reflexively (i.e., no matter the order in which the elements are passed as arguments)."
		// So basically, map will treat any two vectors with the same comparison as equal.
		// eg. it thinks (5,6) == (6,5) because their magnitudes are equal
        return v1.MagnitudeSquared() < v2.MagnitudeSquared();

		/*if (v1.ToString() != v2.ToString())
		{
			return true;
		}

		return false;*/
	}
};

#endif