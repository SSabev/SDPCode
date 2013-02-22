#include "AIControl.h"

#include <vector>

#include "../Shared/SharedMem.h"

#include <string.h>
#include <iostream>
#include <fstream>
#include <algorithm>

/*!
 * Initialises all the components. Called once, either by the GUI or internally.
*/
void AIControl::Initialise()
{
	m_foresee = Foresee();
	m_eagle = Eagle();
	m_aStar = AStar();
	m_impala = Impala();
}

#if defined(STANDALONE)
/*!
 * A special Initialise method used when the AI is running standalone. Allows us to simulate shared memory. Only available when STANDALONE is defined.
*/
void AIControl::Initialise(TShMem* pSharedMemory)
{
	m_pSharedMemory = pSharedMemory;
	
	Initialise();
}
#endif

/*!
 * The key method of AIControl, RunAI should be called to run the entirety of the AI: Foresee, Eagle, AStar and 
 * Impala. It reads data from shared memory (written by the Vision system) and writes data back to shared memory 
 * (for usage by the Navigation system).
 */
void AIControl::RunAI()
{	
#if defined(STANDALONE)

	// Simulate the shared memory.
	TShMem sharedMem = *m_pSharedMemory;

#endif

	int currentFrameIndex = sharedMem.currentIdx;

	TEntry* currentEntry = &sharedMem.positioning[currentFrameIndex];

	RobotState blueRobotState(currentEntry->visionData.blue_x, currentEntry->visionData.blue_y,currentEntry->visionData.blue_angle);
	RobotState yellowRobotState(currentEntry->visionData.yellow_x, currentEntry->visionData.yellow_y, currentEntry->visionData.yellow_angle);

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

	Vector2 ballPos(currentEntry->visionData.ball_x, currentEntry->visionData.ball_y);

	RobotState ourRobotFuture;
	RobotState enemyRobotFuture;
	Vector2 ballFuture;

	// Given the current position and a certain number of previous positions, 
	// approximate where the bot will be when it receives our next transmission.
	m_foresee.SetPitchDimensions(sharedMem.pitchCfg.pitchWidth, sharedMem.pitchCfg.pitchHeight);
	m_foresee.ExtrapolateEnvironment(ourRobot, enemyRobot, ballPos, ourRobotFuture, enemyRobotFuture, ballFuture);

	// Given the positions of the robots and ball, identify the ideal position 
	// and orientation for us to reach.
	m_eagle.SetPitchDimensions(sharedMem.pitchCfg.pitchWidth, sharedMem.pitchCfg.pitchHeight);
	m_eagle.SetState(sharedMem.systemState);
	RobotState targetState = m_eagle.IdentifyTarget(ourRobotFuture, enemyRobotFuture, ballFuture);

	// Using A*, generate the best path to the target.
	m_aStar.SetPitchDimensions(sharedMem.pitchCfg.pitchWidth, sharedMem.pitchCfg.pitchHeight);
	std::list<RobotState> aStarPath = m_aStar.GeneratePath(ourRobotFuture, targetState);

	// If the A* has returned a blank path, it found it impossible to complete.
	if (aStarPath.size() == 0)
	{
		std::string logMessage = "AI could not path-find between " + ourRobotFuture.Position().ToString() + " and " + targetState.Position().ToString();
	
		loggingObj->ShowMsg(logMessage.c_str());
		return;
	}

	std::list<RobotState> smoothedPath; 
	
	// Ensure that the path is long enough to do some smoothing to it.
	if (aStarPath.size() > 2)
	{
		// Smooth and optimise the path using knowledge of our bot's capabilities.
		smoothedPath = m_impala.SmoothPath(aStarPath, 9);
	}
	else 
	{
		smoothedPath = aStarPath;
	}

	// Results should be written to shared memory.
	currentEntry->aiData.pathLength = smoothedPath.size();
	currentEntry->aiData.shouldKick = 0;

	const int maxPathSize = 30;
	const int pointsToWrite = std::min((int)smoothedPath.size(), maxPathSize);
	int pointsWritten = 0;

	std::list<RobotState>::iterator it;

	for (it = smoothedPath.begin(); it!=smoothedPath.end(); it++)
	{
		if (pointsWritten >= maxPathSize)
		{
			break;
		}

		currentEntry->aiData.path[pointsWritten].position_X = it->Position().X();
		currentEntry->aiData.path[pointsWritten].position_Y = it->Position().Y();
		currentEntry->aiData.path[pointsWritten].orientation = it->Orientation();

		pointsWritten++;
	}
}

#if defined(TEST)
/*!
 * Outputs several data files containing points, for use by the plotting/plot_path.sh script - this allows us 
 * to visualise a particular frame of AI output data.
 *
 * Only available when TEST is defined.
*/
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
#endif
