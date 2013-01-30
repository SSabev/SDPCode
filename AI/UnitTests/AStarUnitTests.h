#if !defined(ASTARUNITTESTS_H)
#define ASTARUNITTESTS_H

#include "cpptest.h"

class AStarUnitTests : public Test::Suite
{
public:
	AStarUnitTests();

private:
	void AStarFindAdjacentNodesZero();
	void AStarFindPath();

};

#endif