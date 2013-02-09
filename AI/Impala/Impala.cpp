#include "Impala.h"

#include <cmath>

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

	std::list<Vector2> collapsedPath = CollapsePoints(smoothedPath);

	return collapsedPath;
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

// This tries to reduce the number of waypoints by collapsing sequential ones 
// with a sufficiently similar gradient.
std::list<Vector2> Impala::CollapsePoints(std::list<Vector2> path)
{
	const float GRADIENT_THRESHOLD = 0.1;

	std::vector<Vector2> pathVector(path.begin(), path.end());

	std::list<Vector2> collapsedPath;

	collapsedPath.push_back(pathVector[0]);

	// Size-3 as we need at least three points to collapse one of them.
	// And the last one will be added anyway.
	for (int i=0; i<pathVector.size()-3; i++)
	{
		float originalGradient = pathVector[i].Gradient(&pathVector[i+1]);

		for (int j=i+1; j < pathVector.size()-2; j++)
		{
			float currentGradient = pathVector[j].Gradient(&pathVector[j+1]);
			float gradientDifference = std::abs(currentGradient - originalGradient);

			if (gradientDifference > GRADIENT_THRESHOLD)
			{
				// We can't consume this point, the gradient difference is too large.
				// Add the last successful point to our list and change i so we can start a new loop.
				collapsedPath.push_back(pathVector[j]);
				i=j;
				break;
			}
		}
	}

	collapsedPath.push_back(pathVector[pathVector.size()-1]);
	
	return collapsedPath;
}
