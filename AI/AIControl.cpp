#include "AIControl.h"

#include <vector>

void AIControl::Initialise()
{
	m_foresee = Foresee();
	m_eagle = Eagle();
	m_aStar = AStar();
	m_impala = Impala();
}

void AIControl::RunAI()
{	
	// These should be read from shared memory (sharedMem).
	RobotState ourRobot(0,0,0);
	RobotState enemyRobot(0,0,0);
	Vector2 ballPos(0,0);

	// Given the current position and a certain number of previous positions, 
	// approximate where the bot will be when it receives our next transmission.
	std::vector<Vector2> futurePositions = m_foresee.ExtrapolateState(ourRobot.Position(), enemyRobot.Position(), ballPos);
	
	// Given the positions of the robots and ball, identify the ideal position 
	// and orientation for us to reach.
	Vector2 targetPosition = m_eagle.IdentifyTarget(futurePositions);
	
	// Using A*, generate the best path to the target.
	std::list<Vector2> aStarPath = m_aStar.GeneratePath(futurePositions[0], targetPosition);
	
	// Smooth and optimise the path using knowledge of our bot's capabilities.
	std::list<Vector2> smoothedPath = m_impala.SmoothPath(aStarPath);

	// Results should be written to shared memory.
}