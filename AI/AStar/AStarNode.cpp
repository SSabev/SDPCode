#include "AStarNode.h"

AStarNode::AStarNode()
{
	fCost = MAXFLOAT;
	gCost = MAXFLOAT;
	hCost = MAXFLOAT;
}