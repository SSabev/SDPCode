#include "AIControl.h"

#include "RobotState.h"
#include "MathTypes/Vector2.h"
#include "../Shared/SharedMem.h"

#include <string.h>

#if defined(TEST)
#include "cpptest.h"

#include "UnitTests/AStarUnitTests.h"
#include "UnitTests/Vector2UnitTests.h"
#include "UnitTests/ForeseeUnitTests.h"
#include "UnitTests/AIControlUnitTests.h"
#include "UnitTests/EagleUnitTests.h"
#endif

int main(int argc, char *argv[])
{
#if defined(TEST)

	Test::TextOutput output(Test::TextOutput::Verbose);

	AStarUnitTests aStarUnitTests;
	aStarUnitTests.run(output, true);

	Vector2UnitTests vector2UnitTests;
	vector2UnitTests.run(output, true);
	
	ForeseeUnitTests foreseeUnitTests;
    	foreseeUnitTests.run(output, true);

    	AIControlUnitTests aiControlUnitTests;
   	aiControlUnitTests.run(output, true);
   	
   	EagleUnitTests eagleUnitTests;
    	eagleUnitTests.run(output, true);

#else

	// TODO: Rewrite the testing setup for shared memory changes
	/*TShMem sharedMem;

	AIControl aiControl;

	// zero-out the shared memory
    memset(&sharedMem, 0, sizeof(TShMem));
    // set current state as IDLE
    sharedMem.systemState = eIDLE;
	sharedMem.pitchCfg.pitchWidth = 600;
	sharedMem.pitchCfg.pitchHeight = 300;
	
	aiControl.Initialise(&sharedMem);

	sharedMem.currentIdx = 0;

	sharedMem.positioning[0].visionData.yellow_x = 540;
	sharedMem.positioning[0].visionData.yellow_y = 120;
	sharedMem.positioning[0].visionData.blue_x = 540;
	sharedMem.positioning[0].visionData.blue_y = 120;
	sharedMem.positioning[0].visionData.ball_x = 270;
	sharedMem.positioning[0].visionData.ball_y = 60;

	aiControl.RunAI();	*/

#endif
	
	return 0;
}
