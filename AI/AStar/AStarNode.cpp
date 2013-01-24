#include "AStarNode.h"

#include <float.h>

AStarNode::AStarNode()
{
	fCost = FLT_MAX;
	gCost = FLT_MAX;
	hCost = FLT_MAX;
}