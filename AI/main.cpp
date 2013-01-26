#include "AIControl.h"

#include "RobotState.h"
#include "MathTypes/Vector2.h"

#include "../Shared/SharedMem.h"

#include <string.h>

#if defined(TEST)
#include "cpptest.h"

#include "UnitTests/AStarUnitTests.h"
#include "UnitTests/Vector2UnitTests.h"
#endif

int main(int argc, char *argv[])
{
#if defined(STANDALONE)

	// Simulate the shared memory if we're running standalone.
	TShMem sharedMem;

	// zero-out the shared memory
    memset(&sharedMem, 0, sizeof(TShMem));
    // set current state as IDLE
    sharedMem.systemState = eIDLE;

#endif

#if defined(TEST)

	Test::TextOutput output(Test::TextOutput::Verbose);

	AStarUnitTests aStarUnitTests;
	aStarUnitTests.run(output, true);

	Vector2UnitTests vector2UnitTests;
	vector2UnitTests.run(output, true);

#else

	AIControl aiControl;
	
	aiControl.Initialise();
	aiControl.RunAI();	

#endif
	
	return 0;
}