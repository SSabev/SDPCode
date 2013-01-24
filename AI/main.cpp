/* This is the AI for the bot. It is currently broken down into four separate units:
 * 
 * Foresee - Attempts to work against the delays in data coming from the vision unit and 
 * ultimately being sent to the robot. Does this using linear extrapolation.
 * 
 * Eagle - Decides the high-level action to take. Do we want to get behind the ball?
 * Or do we want to get back to our goal and defend? etc. What position do we need to be in?
 * 
 * AStar - Find a path from the robot's current position to the desired destination 
 * identified by Eagle.
 * 
 * Impala - 
 */

#include "Foresee.h"
#include "Eagle.h"
#include "AStar.h"
#include "Impala.h"

#include "MathTypes/Vector2.h"

#include <vector>

Foresee m_foresee;
Eagle m_eagle;
AStar m_aStar;
Impala m_impala;

void Initialise()
{
	m_foresee = Foresee();
	m_eagle = Eagle();
	m_aStar = AStar();
	m_impala = Impala();
}

int main(int argc, char *argv[])
{
	Vector2 ourRobotPos = Vector2(0,0);
	Vector2 enemyRobotPos = Vector2(0,0);
	Vector2 ballPos = Vector2(0,0);
	
	Initialise();
	
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
	
	return 0;
}