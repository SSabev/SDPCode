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

// This is used as the comparer in maps.
struct Vector2Comparer 
{
    bool operator() (const Vector2 &v1, const Vector2 &v2) const 
	{
        return v1.MagnitudeSquared() < v2.MagnitudeSquared();
	}
};

#endif