#include "Impala.h"

Impala::Impala()
{
	
}

std::list<Vector2> Impala::SmoothPath(std::list<Vector2> aStarPath, int numAdditionalPasses)
{
	std::list<Vector2> smoothedPath = RunPass(aStarPath);

	for (int i=0; i<numAdditionalPasses; i++)
	{
		smoothedPath = RunPass(smoothedPath);
	}

	return smoothedPath;
}

// New triangle centroid method.
std::list<Vector2> Impala::RunPass(std::list<Vector2> path)
{
	std::vector<Vector2> pathVector(path.begin(), path.end());

	// This method attempts to find the centroid of a triangle, where the three points 
	// are a given one and those immediately preceding and following it in the path.
	std::list<Vector2> smoothedPath;

	smoothedPath.push_front(pathVector.front());

	// Note  1 <= i < size-1, as we want a point either side of it.
	for (int i=1; i<pathVector.size()-1; i++)
	{
		Vector2 midpoint = (pathVector[i-1] + pathVector[i] + pathVector[i+1])/3;
	
		smoothedPath.push_back(midpoint);
	}

	smoothedPath.push_back(pathVector.back());

	return smoothedPath;
}

// Original Method
/*std::list<Vector2> Impala::RunPass(std::list<Vector2> path)
{
	std::vector<Vector2> pathVector(path.begin(), path.end());

	// Currently, our smoothing is going to be as simple as finding the centre 
	// between each pair of points and using that as part of the smoothed path.
	std::list<Vector2> smoothedPath;

	smoothedPath.push_front(pathVector.front());

	// Note the size-1, as we only want pairs.
	for (int i=0; i<pathVector.size()-1; i++)
	{
		Vector2 midpointOffset = (pathVector[i+1] - pathVector[i])/2;

		Vector2 smoothedPoint = pathVector[i] + midpointOffset;
	
		smoothedPath.push_back(smoothedPoint);
	}

	smoothedPath.push_back(pathVector.back());

	return smoothedPath;
}*/