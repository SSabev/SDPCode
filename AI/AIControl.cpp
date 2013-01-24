#include "AIControl.h"

#include "MathTypes/Vector2.h"

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
	Vector2 ourRobotPos(0,0);
	Vector2 enemyRobotPos(0,0);
	Vector2 ballPos(0,0);
	
	// Given the current position and a certain number of previous positions, 
	// approximate where the bot will be when it receives our next transmission.
	std::vector<Vector2> futurePositions = m_foresee.ExtrapolateState(ourRobotPos, enemyRobotPos, ballPos);
	
	// Given the positions of the robots and ball, identify the ideal position 
	// and orientation for us to reach.
	Vector2 targetPosition = m_eagle.IdentifyTarget(futurePositions);
	
	// Using A*, generate the best path to the target.
	std::vector<Vector2> aStarPath = m_aStar.GeneratePath(futurePositions[0], targetPosition);
	
	// Smooth and optimise the path using knowledge of our bot's capabilities.
	std::vector<Vector2> smoothedPath = m_impala.SmoothPath(aStarPath);
}