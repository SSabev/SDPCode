#if !defined(ASTARNODE_H)
#define ASTARNODE_H

#include "../MathTypes/Vector2.h"

class AStarNode
{
public:
	AStarNode();

	const float getFScore();
	const float getGScore();
	void setGScore(float gScore);
	void setHScore(float hScore);
	void setBias(float bias);

	const Vector2 getPreviousNode();
	void setPreviousNode(Vector2 previousNode);

private:
	// fScore = gScore + hScore
	float m_fScore;
	// gScore is the cost of travelling from the starting node to this one.
	float m_gScore;
	// hScore is the admissable heuristic. In this case, the absolute distance from this node to the destination.
	float m_hScore;
	// This is the path node which lead us to this one.
	Vector2 m_previousNode;
	// This is used to make a node more or less attractive to the A*.	
	// Positive values here make the node less attractive, negative values make it more attractive.
	float m_bias;
};

#endif