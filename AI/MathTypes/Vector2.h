#if !defined(VECTOR2_H)
#define VECTOR2_H

/* Vector2 represents a two-dimensional vector. 
 * This is useful for positions of entities.
 */
class Vector2
{
public:
	Vector2(float x, float y);
	
	float X() 
	{
		return m_x;
	};
	
	float Y() 
	{
		return m_y;
	};
	
private:
	float m_x;
	float m_y;
};

#endif