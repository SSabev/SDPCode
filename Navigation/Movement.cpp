/*
 * movement.cpp
 *
 *  Created on: Jan 25, 2013
 *      Author: Georgi
 */
#include "Movement.h"
#include "Motor.h"
#include "Compute.h"
#include "../Shared/Logging.h"
#include "../Shared/SharedMem.h"
//#include "MathTypes/Vector2.h"
#include <QString>
#include <string.h>

Movement::Movement()
{


	//Get AI data from shared memory for now I'll set it to a hardcoded value for testing purposes untill I get the exact data from AI
	//set up test vectors

	Vector2 targetLocation = Vector2(0,0);
	Vector2 ourRobotLocation = Vector2(0,5);

	//Issue the speeds to be written into shared memory
    	IssueSpeed(targetLocation,ourRobotLocation);
	

    	sharedMem.positioning[sharedMem.currentIdx].robotData.motor_1 = GetMotorSpeed(1);
        sharedMem.positioning[sharedMem.currentIdx].robotData.motor_2 = GetMotorSpeed(2);
        sharedMem.positioning[sharedMem.currentIdx].robotData.motor_3 = GetMotorSpeed(3);
        sharedMem.positioning[sharedMem.currentIdx].robotData.motor_4 = GetMotorSpeed(4);

        loggingObj->ShowMsg(QString("speed1 %1, speed2 %2, speed3 %3, speed4 %4")
                            .arg(GetMotorSpeed(1))
                            .arg(GetMotorSpeed(2))
                            .arg(GetMotorSpeed(3))
                            .arg(GetMotorSpeed(4))
                            .toAscii()
                            .data());


}

Movement::Movement(Vector2 targetLocation, Vector2 ourRobotLocation)
{


         IssueSpeed(targetLocation,ourRobotLocation);
	 
         sharedMem.positioning[sharedMem.currentIdx].robotData.motor_1 = GetMotorSpeed(1);
         sharedMem.positioning[sharedMem.currentIdx].robotData.motor_2 = GetMotorSpeed(2);
         sharedMem.positioning[sharedMem.currentIdx].robotData.motor_3 = GetMotorSpeed(3);
         sharedMem.positioning[sharedMem.currentIdx].robotData.motor_4 = GetMotorSpeed(4);

    	 loggingObj->ShowMsg(QString("speed1 %1, speed2 %2, speed3 %3, speed4 %4")
                            .arg(GetMotorSpeed(1))
                            .arg(GetMotorSpeed(2))
                            .arg(GetMotorSpeed(3))
                            .arg(GetMotorSpeed(4))
                            .toAscii()
                            .data());

	
}

void Movement::IssueSpeed(Vector2 targetLocation, Vector2 ourRobotLocation)
{
Compute comp = Compute();
int angleToTurn = comp.ComputeOrientationAngle(targetLocation, ourRobotLocation);

if(angleToTurn>=0)
    {
        comp.ComputeMotorSpeeds(angleToTurn);
        motor1.SetSpeed(comp.GetMotorSpeed(0));
        motor2.SetSpeed(comp.GetMotorSpeed(1));
        motor3.SetSpeed(comp.GetMotorSpeed(2));
        motor4.SetSpeed(comp.GetMotorSpeed(3));

    }
else
    {
        angleToTurn = (-1) * angleToTurn;
        comp.ComputeMotorSpeeds(angleToTurn);
        motor1.SetSpeed((-1) * comp.GetMotorSpeed(0));
        motor2.SetSpeed((-1) * comp.GetMotorSpeed(1));
        motor3.SetSpeed((-1) * comp.GetMotorSpeed(2));
        motor4.SetSpeed((-1) * comp.GetMotorSpeed(3));
    }
}

int Movement::GetMotorSpeed(int motorNumber)
{
    if(motorNumber == 1) return motor2.GetSpeed();
    if(motorNumber == 2) return motor4.GetSpeed();
    if(motorNumber == 3) return motor1.GetSpeed();
    if(motorNumber == 4) return motor3.GetSpeed();
	return 0;
	
}



