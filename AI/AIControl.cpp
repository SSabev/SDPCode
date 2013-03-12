#include "AIControl.h"

#include <vector>

#include "../Shared/SharedMem.h"

#include <cmath>
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

	// Check that the data that's coming in from shared memory is correct and can be used.
	if (IsFailedFrame(ourRobot))
	{
		// In this case, the data from vision can't be used.
		std::string logMessage = "AI believes that data from Vision is bad and can't be used.";
	
		loggingObj->ShowMsg(logMessage.c_str());

		currentEntry->aiData.isFailedFrame = 1;
		return;
	}

	bool isBallPositionBad = false;
	bool doWeHaveBall = false;

	// Check if the ball position is valid and can be used, or if we have to rely on last known position.
	/*if (!CoordinatesAreBad(ballPos))
	{
		m_lastKnownBallPosition = ballPos;
		isBallPositionBad = false;
	}
	else
	{
		isBallPositionBad = true;

		if (m_lastKnownBallPosition.IsSet())
		{
			ballPos = m_lastKnownBallPosition;
		}
		else
		{
			// In this case, the data from vision can't be used.
			std::string logMessage = "AI believes that data from Vision is bad and can't be used.";
		
			loggingObj->ShowMsg(logMessage.c_str());

			currentEntry->aiData.isFailedFrame = 1;
			return;
		}*/

	if (CoordinatesAreBad(ballPos))
	{
		// For the milestone, we're assuming that if we can't see the ball, we have it.
		doWeHaveBall = true;
	}

	RobotState ourRobotFuture;
	RobotState enemyRobotFuture;
	Vector2 ballFuture;

	// Given the current position and a certain number of previous positions, 
	// approximate where the bot will be when it receives our next transmission.
	m_foresee.SetPitchDimensions(sharedMem.pitchCfg.pitchWidth, sharedMem.pitchCfg.pitchHeight);
	m_foresee.ExtrapolateEnvironment(ourRobot, enemyRobot, ballPos, ourRobotFuture, enemyRobotFuture, ballFuture);

	// Given the positions of the robots and ball, identify the ideal position 
	// and orientation for us to reach.
	m_eagle.SetSharedData(sharedMem.systemState, sharedMem.pitchCfg.pitchWidth, sharedMem.pitchCfg.pitchHeight, sharedMem.pitchSide);
	m_eagle.SetHadBallLastFrame(m_hadBallLastFrame, isBallPositionBad);
	RobotState targetState = m_eagle.IdentifyTarget(ourRobotFuture, enemyRobotFuture, ballFuture);

	// Check if we have the ball and should kick from our current state.
	if (m_eagle.DoWeHaveBall(ourRobotFuture, ballFuture))
	{
		doWeHaveBall = true;
	}

	bool shouldKick = m_eagle.ShouldWeShoot(ourRobotFuture, enemyRobotFuture, ballFuture) && doWeHaveBall;

	// Set if we have the ball this frame, to be used next frame.
	if (doWeHaveBall)
	{
		m_hadBallLastFrame = true;
	}
	else
	{
		m_hadBallLastFrame = false;
	}

	// Using A*, generate the best path to the target.
	m_aStar.SetSharedData(sharedMem.pitchCfg.pitchWidth, sharedMem.pitchCfg.pitchHeight, sharedMem.pitchSide);
	std::list<RobotState> aStarPath = m_aStar.GeneratePath(ourRobotFuture, targetState, doWeHaveBall, ballFuture, enemyRobotFuture);

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
	currentEntry->aiData.isFailedFrame = 0;
	currentEntry->aiData.shouldKick = shouldKick;
	currentEntry->aiData.doWeHaveBall = doWeHaveBall;

    //DEBUG
    if (doWeHaveBall)
    {
        std::string logMessage = "I have the ball.";

        loggingObj->ShowMsg(logMessage.c_str());
    }
    else
    {
        std::string logMessage = "I do NOT have the ball.";

        loggingObj->ShowMsg(logMessage.c_str());
    }

	const int maxPathSize = 30;
	const int pointsToWrite = std::min((int)smoothedPath.size(), maxPathSize);
	currentEntry->aiData.pathLength = pointsToWrite;

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
		currentEntry->aiData.path[pointsWritten].orientation = WrapValue(it->Orientation());

		pointsWritten++;
	}
}

/*!
 * Given some Vector2 object, this CoordinatesAreBad method checks whether the X and Y positions are within 0-(pitch dimensions) inclusive. Returns true if some coordinate is invalid, false if coordinates are valid.
 */
bool AIControl::CoordinatesAreBad(Vector2 objectPosition)
{
#if defined(STANDALONE)
	
	// Simulate the shared memory if we're in standalone mode
	TShMem sharedMem = *m_pSharedMemory;

#endif

	int pitchSizeX = sharedMem.pitchCfg.pitchWidth - 1;
	int pitchSizeY = sharedMem.pitchCfg.pitchHeight - 1;
	
	if(objectPosition.X() < 0 || objectPosition.X() > pitchSizeX || objectPosition.Y() < 0 || objectPosition.Y() > pitchSizeY)
	{
		return true;
	}
	
	return false;
}

/*!
 * Wrap ma values yo.
 */
float AIControl::WrapValue(float orientation) {
	
	// orientation = orientation < 0 ? -orientation : orientation ;
	// just kidding, Richard... although the above is totally fabs.
		
	while (orientation < 0)
	{
		orientation+=2*M_PI;
	}
	
	return fmod(orientation, 2*M_PI);
}

/*!
 * Passes the (X,Y) coordinates of robot1, robot2 and ball objects into the CoordinatesAreBad method, returning true if any one coordinate is invalid.
 */
bool AIControl::IsFailedFrame(RobotState ourRobot) {
	
	bool isInvalidRobot = AIControl::CoordinatesAreBad(ourRobot.Position());

	return isInvalidRobot;
}

#if defined(STANDALONE)
/*!
 * Outputs several data files containing points, for use by the plotting/plot_path.sh script - this allows us 
 * to visualise a particular frame of AI output data.
 *
 * Only available when TEST is defined.
*/
void AIControl::Plot(std::list<RobotState> aStarPath, std::vector<RobotState> ourPrevious, RobotState destination, std::vector<Vector2> ballPrevious, Vector2 ballFuture, std::vector<RobotState> enemyRobotPrevious, RobotState enemyRobotFuture)
{
	std::ofstream myfile;
	myfile.open("astar_path.dat");

	std::list<RobotState>::iterator it;

	for (it = aStarPath.begin(); it != aStarPath.end(); it++)
	{
		// We want to make sure that every point except the last one is followed 
		// by a line break.
		if (it != aStarPath.begin())
		{
			myfile << std::endl;
		}

		myfile << it->Position().ToString();
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

	myfile << destination.Position().ToString();

	myfile.close();

	myfile.open("our_current.dat");

	myfile << ourPrevious[1].Position().ToString() << std::endl;
	myfile << ourPrevious[0].Position().ToString() << std::endl;
	myfile << aStarPath.front().Position().ToString();

	myfile.close();

	myfile.open("our_future.dat");

	myfile << aStarPath.front().Position().ToString();

	myfile.close();

	myfile.open("enemy_current.dat");

	myfile << enemyRobotPrevious[1].Position().ToString() << std::endl;
	myfile << enemyRobotPrevious[0].Position().ToString() << std::endl;
	myfile << enemyRobotFuture.Position().ToString();

	myfile.close();

	myfile.open("enemy_future.dat");

	myfile << enemyRobotFuture.Position().ToString();

	myfile.close();
}
#endif