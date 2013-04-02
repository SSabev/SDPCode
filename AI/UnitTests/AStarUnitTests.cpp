#include "AStarUnitTests.h"

#include "../AIControl.h"
#include "../Foresee/Foresee.h"
#include "../AStar/AStar.h"
#include "../AStar/AStarNode.h"
#include "../Impala/Impala.h"

#include "../MathTypes/Vector2.h"

#include "../../Shared/SharedMem.h"

#include <list>
#include <iostream>
#include <cmath>

AStarUnitTests::AStarUnitTests()
{
	TEST_ADD(AStarUnitTests::AStarFindAdjacentNodesZero);
	TEST_ADD(AStarUnitTests::AStarPlot);
}

void AStarUnitTests::AStarFindAdjacentNodesZero()
{
	AStar aStar;

	aStar.SetSharedData(244, 122, eLeftSide);
	std::list<Vector2> adjacentNodes = aStar.FindAdjacentNodes(Vector2(0,0));

	// Nodes adjacent to (0,0) should be (1,0), (0,1), (1,1).
	TEST_ASSERT(adjacentNodes.size() == 3);
}

void AStarUnitTests::AStarPlot()
{
		RobotState ourRobotCurrent(Vector2(440,180), 0.0f);
		RobotState enemyRobotCurrent(Vector2(200, 160), 0.0f);
		Vector2 ballCurrent(80, 90);

		std::vector<RobotState> ourRobotPrevious;
		ourRobotPrevious.push_back(ourRobotCurrent);
		ourRobotPrevious.push_back(RobotState(Vector2(460,200), 0.0f));

		std::vector<RobotState> enemyRobotPrevious;
		enemyRobotPrevious.push_back(enemyRobotCurrent);
		enemyRobotPrevious.push_back(RobotState(Vector2(200,160), 0.0f));

		std::vector<Vector2> ballPrevious;
		ballPrevious.push_back(ballCurrent);
		ballPrevious.push_back(Vector2(80,50));

		AIControl aiControl;
		Foresee foresee;
		AStar aStar;
		Impala impala;
		Eagle eagle;

		// We're not dealing with the enemy robot position currently.
		foresee.SetPitchDimensions(600, 300);
		/*RobotState ourRobotFuture = foresee.ExtrapolateRobotState(ourRobotPrevious);
		RobotState enemyRobotFuture = foresee.ExtrapolateRobotState(enemyRobotPrevious);
		Vector2 ballFuture = foresee.ExtrapolatePosition(ballPrevious);*/

		RobotState ourRobotFuture(450, 150, 0);
		RobotState enemyRobotFuture(580, 200, M_PI/2);
		Vector2 ballFuture(500, 150);

		eagle.SetSharedData(ePenaltyAttack,600,300,eLeftSide);
		bool isMovingToBall = false;
		RobotState targetState = eagle.IdentifyTarget(ourRobotFuture,enemyRobotFuture,ballFuture,isMovingToBall);

		aStar.SetSharedData(600, 300, eLeftSide);
		std::list<RobotState> aStarPath = aStar.GeneratePath(ourRobotFuture, targetState, false, ballFuture, enemyRobotFuture);

			// If we're turning but not changing position, A* won't account for this. 
	// Need to manually add another point in this situation. Could be done more cleanly than this.
	if ((ourRobotFuture.Position() == targetState.Position()) && (ourRobotFuture.Orientation() != targetState.Orientation()))
	{
		aStarPath.push_back(targetState);
	}

		RobotState front = aStarPath.front();
		RobotState back = aStarPath.back();

		std::list<RobotState> smoothedPath = impala.SmoothPath(aStarPath, 99);

		// We're passing ballFuture twice as it also happens to be the destination currently.
		aiControl.Plot(smoothedPath, ourRobotPrevious, targetState, ballPrevious, ballFuture, enemyRobotPrevious, enemyRobotFuture);
}
