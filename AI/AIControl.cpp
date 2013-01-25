#include "AIControl.h"

#include <vector>

void AIControl::Initialise()
{
	m_foresee = Foresee();
	m_eagle = Eagle();
	m_aStar = AStar();
	m_impala = Impala();
}

std::list<RobotState> AIControl::RunAI(RobotState ourRobot, RobotState enemyRobot, Vector2 ballPos)
{	
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
}