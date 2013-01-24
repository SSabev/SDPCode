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
	
	float X();
	float Y();

	void Set(float x, float y);
	float DistanceSquared(Vector2* dest);
	float Distance(Vector2 dest);
	
private:
	float m_x;
	float m_y;
	bool m_isSet;
};

#endif