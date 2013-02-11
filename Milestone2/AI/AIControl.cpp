#include "AIControl.h"

#include <vector>

#include "../Shared/SharedMem.h"

#include <string.h>
#include <iostream>
#include <fstream>
#include <algorithm>

void AIControl::Initialise()
{
#if defined(STANDALONE)
	// zero-out the shared memory
    memset(&sharedMem, 0, sizeof(TShMem));
    // set current state as IDLE
    sharedMem.systemState = eIDLE;
#endif

	m_foresee = Foresee();
	m_eagle = Eagle();
	m_aStar = AStar();
	m_impala = Impala();
}

void AIControl::RunAI()
{	
	int currentFrameIndex = sharedMem.currentIdx;

	TEntry currentEntry = sharedMem.positioning[currentFrameIndex];

	RobotState blueRobotState(currentEntry.visionData.blue_x, currentEntry.visionData.blue_y,currentEntry.visionData.blue_angle);
	RobotState yellowRobotState(currentEntry.visionData.yellow_x, currentEntry.visionData.yellow_y, currentEntry.visionData.yellow_angle);

	RobotState ourRobot;
	RobotState enemyRobot;

	if (sharedMem.teamColor == eBlueTeam)
	{
		ourRobot = blueRobotState;
		enemyRobot = yellowRobotState;
	}
	else
	{
		ourRobot = yellowRobotState;
		enemyRobot = blueRobotState;
	}

	Vector2 ballPos(currentEntry.visionData.ball_x, currentEntry.visionData.ball_y);

	// Given the current position and a certain number of previous positions, 
	// approximate where the bot will be when it receives our next transmission.
	m_foresee.SetPitchDimensions(sharedMem.pitchCfg.pitchWidth, sharedMem.pitchCfg.pitchHeight);
	std::vector<Vector2> futurePositions = m_foresee.ExtrapolateState(ourRobot.Position(), enemyRobot.Position(), ballPos);

	// Given the positions of the robots and ball, identify the ideal position 
	// and orientation for us to reach.
	Vector2 targetPosition = m_eagle.IdentifyTarget(futurePositions);
	
	// Using A*, generate the best path to the target.
	m_aStar.SetPitchDimensions(sharedMem.pitchCfg.pitchWidth, sharedMem.pitchCfg.pitchHeight);
	std::list<Vector2> aStarPath = m_aStar.GeneratePath(futurePositions[0], targetPosition);
	
    std::list<Vector2> smoothedPath;

    if (aStarPath.size() != 0)
    {
        // Smooth and optimise the path using knowledge of our bot's capabilities.
        smoothedPath = m_impala.SmoothPath(aStarPath, 9);
    }
    else
    {
        smoothedPath = aStarPath;
    }

	// Results should be written to shared memory.
	currentEntry.aiData.pathLength = smoothedPath.size();
	currentEntry.aiData.shouldKick = 0;

	const int maxPathSize = 30;
	const int pointsToWrite = std::min((int)smoothedPath.size(), maxPathSize);
	int pointsWritten = 0;

	std::list<Vector2>::iterator it;

	for (it = smoothedPath.begin(); it!=smoothedPath.end(); it++)
	{
		if (pointsWritten >= maxPathSize)
		{
			break;
		}

		currentEntry.aiData.path[pointsWritten].position_X = it->X();
		currentEntry.aiData.path[pointsWritten].position_X = it->Y();
		// For now AI just returns orientation=0.
		currentEntry.aiData.path[pointsWritten].orientation = 0;

		pointsWritten++;
	}
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
