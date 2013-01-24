#include "AIControl.h"

#include "RobotState.h"
#include "MathTypes/Vector2.h"

int main(int argc, char *argv[])
{
	AIControl aiControl;
	
	aiControl.Initialise();
	aiControl.RunAI(RobotState(0,0,0), RobotState(0,0,0), Vector2(0,0));	
	
	return 0;
}