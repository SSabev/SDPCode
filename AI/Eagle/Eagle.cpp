#include "Eagle.h"

Eagle::Eagle()
{
	
}

Vector2 Eagle::IdentifyTarget(std::vector<Vector2> positions)
{
	// For now, this is just the ball position.
	return positions[2];
}