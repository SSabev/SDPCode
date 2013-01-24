#if !defined(ASTARNODE_H)
#define ASTARNODE_H

class AStarNode
{
public:
	AStarNode();

private:
	// fCost = gCost + hCost
	float fCost;
	// gCost is the cost of travelling from the starting node to this one.
	float gCost;
	// hCost is the admissable heuristic. In this case, the absolute distance from this node to the destination.
	float hCost;
};

#endif