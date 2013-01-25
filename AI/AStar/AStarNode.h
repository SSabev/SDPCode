#if !defined(ASTARNODE_H)
#define ASTARNODE_H

class AStarNode
{
public:
	AStarNode();

	const float getFScore();
	const float getGScore();
	void setGScore(float gScore);
	void setHScore(float hScore);

private:
	// fScore = gScore + hScore
	float m_fScore;
	// gScore is the cost of travelling from the starting node to this one.
	float m_gScore;
	// hScore is the admissable heuristic. In this case, the absolute distance from this node to the destination.
	float m_hScore;
};

#endif