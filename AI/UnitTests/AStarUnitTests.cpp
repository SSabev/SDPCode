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
	RobotState ourRobotCurrent(Vector2(15,15), 0.0f);
	Vector2 ballCurrent(230, 65);

	std::vector<RobotState> ourRobotPrevious;
	ourRobotPrevious.push_back(ourRobotCurrent);
	ourRobotPrevious.push_back(RobotState(Vector2(5,5), 0.34906585f));

	std::vector<Vector2> ballPrevious;
	ballPrevious.push_back(ballCurrent);
	ballPrevious.push_back(Vector2(210,85));

	AIControl aiControl;
	Foresee foresee;
	AStar aStar;
	Impala impala;

	// We're not dealing with the enemy robot position currently.
	foresee.SetPitchDimensions(244, 122);
	RobotState ourRobotFuture = foresee.ExtrapolateRobotState(ourRobotPrevious);
	Vector2 ballFuture = foresee.ExtrapolatePosition(ballPrevious);

	aStar.SetPitchDimensions(244, 122);
	std::list<RobotState> aStarPath = aStar.GeneratePath(ourRobotFuture, RobotState(ballFuture,0));

	std::list<RobotState> smoothedPath = impala.SmoothPath(aStarPath, 19);

	// We're passing ballFuture twice as it also happens to be the destination currently.
	aiControl.Plot(smoothedPath, ourRobotPrevious, RobotState(ballFuture,0), ballPrevious, ballFuture);
}
