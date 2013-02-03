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

std::list<Vector2> Impala::RunPass(std::list<Vector2> path)
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
}