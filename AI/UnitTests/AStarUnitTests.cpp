#include "AStarUnitTests.h"

#include "../AStar/AStar.h"
#include "../AStar/AStarNode.h"

#include "../MathTypes/Vector2.h"

#include <list>
#include <iostream>

AStarUnitTests::AStarUnitTests()
{
	TEST_ADD(AStarUnitTests::AStarFindAdjacentNodesZero);
	TEST_ADD(AStarUnitTests::AStarFindPath);
}

void AStarUnitTests::AStarFindAdjacentNodesZero()
{
	AStar aStar;

	std::list<Vector2> adjacentNodes = aStar.FindAdjacentNodes(Vector2(0,0));

	// Nodes adjacent to (0,0) should be (1,0), (0,1), (1,1).
	TEST_ASSERT(adjacentNodes.size() == 3);
}

void AStarUnitTests::AStarFindPath()
{
	AStar aStar;

	std::list<Vector2> aStarPath = aStar.GeneratePath(Vector2(5,60), Vector2(18,90));

	std::list<Vector2>::iterator it;

	for (it = aStarPath.begin(); it != aStarPath.end(); it++)
	{
		std::cout << it->ToString() << std::endl;
	}

	//TEST_ASSERT(aStarPath.size() == 11);
}