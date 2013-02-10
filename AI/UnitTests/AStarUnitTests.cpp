#include "AStarUnitTests.h"

#include "../AIControl.h"
#include "../Foresee/Foresee.h"
#include "../AStar/AStar.h"
#include "../AStar/AStarNode.h"
#include "../Impala/Impala.h"

#include "../MathTypes/Vector2.h"

#include <list>
#include <iostream>

AStarUnitTests::AStarUnitTests()
{
	TEST_ADD(AStarUnitTests::AStarFindAdjacentNodesZero);
	TEST_ADD(AStarUnitTests::AStarPlot);
}

void AStarUnitTests::AStarFindAdjacentNodesZero()
{
	AStar aStar;

	aStar.SetPitchDimensions(244, 122);
	std::list<Vector2> adjacentNodes = aStar.FindAdjacentNodes(Vector2(0,0));

	// Nodes adjacent to (0,0) should be (1,0), (0,1), (1,1).
	TEST_ASSERT(adjacentNodes.size() == 3);
}

void AStarUnitTests::AStarPlot()
{
	Vector2 ourRobotCurrent(15,15);
	Vector2 ballCurrent(230, 65);

	std::vector<Vector2> ourRobotPrevious;
	ourRobotPrevious.push_back(ourRobotCurrent);
	ourRobotPrevious.push_back(Vector2(5,5));

	std::vector<Vector2> ballPrevious;
	ballPrevious.push_back(ballCurrent);
	ballPrevious.push_back(Vector2(210,85));

	AIControl aiControl;
	Foresee foresee;
	AStar aStar;
	Impala impala;

	// We're not dealing with the enemy robot position currently.
	foresee.SetPitchDimensions(244, 122);
	Vector2 ourRobotFuture = foresee.ExtrapolatePositionFromPoints(ourRobotPrevious);
	Vector2 ballFuture = foresee.ExtrapolatePositionFromPoints(ballPrevious);

	aStar.SetPitchDimensions(244, 122);
	std::list<Vector2> aStarPath = aStar.GeneratePath(ourRobotFuture, ballFuture);

	std::list<Vector2> smoothedPath = impala.SmoothPath(aStarPath, 19);

	// We're passing ballFuture twice as it also happens to be the destination currently.
	aiControl.Plot(smoothedPath, ourRobotPrevious, ballFuture, ballPrevious, ballFuture);
}
