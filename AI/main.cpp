#include "AIControl.h"

#include "RobotState.h"
#include "MathTypes/Vector2.h"

#if defined(TEST)
#include "cpptest.h"

#include "UnitTests/Vector2UnitTests.h"
#endif

int main(int argc, char *argv[])
{
	#if defined(TEST)

	Vector2UnitTests vector2UnitTests;
	Test::TextOutput output(Test::TextOutput::Verbose);
	vector2UnitTests.run(output, true);

	#else

	AIControl aiControl;
	
	aiControl.Initialise();
	aiControl.RunAI(RobotState(0,0,0), RobotState(0,0,0), Vector2(0,0));	

	#endif
	
	return 0;
}