#if !defined(VECTOR2_H)
#define VECTOR2_H

/* Vector2 represents a two-dimensional vector. 
 * This is useful for positions of entities.
 */
class Vector2
{
public:
	Vector2();
	Vector2(float x, float y);
	void Set(float x, float y);
	
	float X() const;
	float Y() const;

	float Distance(Vector2* dest) const;
	float DistanceSquared(Vector2* dest) const;
	float MagnitudeSquared() const;

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
        return v1.MagnitudeSquared() < v2.MagnitudeSquared();
	}
};

#endif