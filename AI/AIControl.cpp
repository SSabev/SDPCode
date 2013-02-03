#include "AIControl.h"

#include <vector>

#include <iostream>
#include <fstream>

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

void AIControl::Plot(std::list<Vector2> aStarPath, std::vector<Vector2> ourPrevious, Vector2 destination, std::vector<Vector2> ballPrevious, Vector2 ballFuture)
{
	std::ofstream myfile;
	myfile.open("astar_path.dat");

	std::list<Vector2>::iterator it;

	for (it = aStarPath.begin(); it != aStarPath.end(); it++)
	{
		// We want to make sure that every point except the last one is followed 
		// by a line break.
		if (it != aStarPath.begin())
		{
			myfile << std::endl;
		}

		myfile << it->ToString();
	}

	myfile.close();

	myfile.open("ball_current.dat");

	myfile << ballPrevious[1].ToString() << std::endl;
	myfile << ballPrevious[0].ToString() << std::endl;
	myfile << ballFuture.ToString();

	myfile.close();

	myfile.open("ball_future.dat");

	myfile << ballFuture.ToString();

	myfile.close();

	myfile.open("dest_pos.dat");

	myfile << destination.ToString();

	myfile.close();

	myfile.open("our_current.dat");

	myfile << ourPrevious[1].ToString() << std::endl;
	myfile << ourPrevious[0].ToString() << std::endl;
	myfile << aStarPath.front().ToString();

	myfile.close();

	myfile.open("our_future.dat");

	myfile << aStarPath.front().ToString();

	myfile.close();
}