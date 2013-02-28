#include "Navigation_Holonomic.h"


#include <Logging.h>
#include "QString"
#include <string.h>
#include <cmath>

#define _USE_MATH_DEFINES

/// These values for 100ms and for 1st milestone type robot
#define MAX_DISPLACEMENT_PX     (3.0)
#define MAX_DISPLACEMENT_PX_SQ  (MAX_DISPLACEMENT_PX * MAX_DISPLACEMENT_PX)
#define ANGULAR_THRESHOLD       (M_PI_4) // 45 degrees

#define MAX_ONLY_ANGULAR_MSPEED (60)
#define MAX_ALGULAR_VEL_100ms   (0.12)
#define MAX_FORWARD_MSPEED      (50)
#define MAX_ANGULAR_MSPEED      (30)
#define ANGULAR_RATIO           (MAX_ANGULAR_MSPEED / MAX_ALGULAR_VEL_100ms)

#define ROBOT_D                 (13) // cm
#define WEEL_D                  (3)  // cm
#define SPEED_COEFFICIENT       (2*ROBOT_D*10/WEEL_D)
#define MAX_SPEED               (300)
#define SCALING_FACTOR          (10)

typedef enum{
    eTurnLeft,
    eTurnRight
} TTurnDir;

CNavigation::CNavigation()
{
    memset(&m_target, 0, sizeof(TTarget));
}

void CNavigation::RotateToOrient(float targetOrient)
{
    TEntry *entry = &sharedMem.positioning[sharedMem.currentIdx];
     memset(&entry->robotData, 0, sizeof(TRobotData));
	//check our orientation and flip rotation if facing the wrong side
	if(m_ourOrientation <= M_PI_2 || m_ourOrientation > 3* M_PI_2)	
	{
	if(targetOrient - m_ourOrientation < 0)
	//rotate left
	if (targetOrient - m_ourOrientation > ANGULAR_THRESHOLD){
	entry->robotData.motor_left =  -100;
        entry->robotData.motor_right =  100;
        entry->robotData.motor_front =  100;
        entry->robotData.motor_rear =  -100;
	}
	else
	//rotate right	
	if (targetOrient - m_ourOrientation > ANGULAR_THRESHOLD){
	entry->robotData.motor_left =    100;
        entry->robotData.motor_right =  -100;
        entry->robotData.motor_front =  -100;
        entry->robotData.motor_rear =    100;
	}
	}
	else{
	if(targetOrient - m_ourOrientation < 0)
	//rotate right
	if (targetOrient - m_ourOrientation > ANGULAR_THRESHOLD){
	entry->robotData.motor_left =    100;
        entry->robotData.motor_right =  -100;
        entry->robotData.motor_front =  -100;
        entry->robotData.motor_rear =    100;
	}
	else
	//rotate left
	if (targetOrient - m_ourOrientation > ANGULAR_THRESHOLD){
	entry->robotData.motor_left =   -100;
        entry->robotData.motor_right =   100;
        entry->robotData.motor_front =   100;
        entry->robotData.motor_rear =   -100;
	}
	}

}

void CNavigation::Holonomic()
{
    TEntry *entry = &sharedMem.positioning[sharedMem.currentIdx];

    if(sharedMem.teamColor == eYellowTeam){
        m_ourPos_x = (float) entry->visionData.yellow_x;
        m_ourPos_y = (float) entry->visionData.yellow_y;
        m_ourOrientation =entry->aiData.path[0].orientation;
                //entry->visionData.yellow_angle;
    }
    else{
        m_ourPos_x = (float) entry->visionData.blue_x;
        m_ourPos_y = (float) entry->visionData.blue_y;
        m_ourOrientation = entry->aiData.path[0].orientation;
                //entry->visionData.blue_angle;
    }
    if(m_ourOrientation <0)
        m_ourOrientation = 2*M_PI + m_ourOrientation;
    //m_ourOrientation = entry->aiData.path[0].orientation;
    memset(&entry->robotData, 0, sizeof(TRobotData));
    //GenerateLinear(entry);
    int dx;
    int dy;
    float theta;
    int  motorSpeed[4];
    float angle = 0;
    float old_angle;
    dx = (int)entry->aiData.path[1].position_X - (int)entry->aiData.path[0].position_X;
    dy = (int)entry->aiData.path[1].position_Y - (int)entry->aiData.path[0].position_Y;

    theta = atan2(dy,dx);
    entry->aiData.path[1].orientation = theta;
    // if(m_ourOrientation <= M_PI_2 || m_ourOrientation > 3* M_PI_2)
    //generate angle difference and normalise in degrees
    angle = (theta - m_ourOrientation) -M_PI_2;
    //else
    //angle = M_PI + (theta - m_ourOrientation);

    old_angle = angle;
     if(angle<0)
     {
         angle = 2*M_PI+angle;
     }

        //entry->aiData.path[1].orientation = angle;
     loggingObj->ShowMsg(QString("angle to target : %1")
                             .arg(angle)

             .toAscii()
                             .data());
    angle = angle*180/M_PI;
    int speed1, speed2;
    if (angle <= 90) {
        if (angle > (90 - angle)) {
        speed1 = 100;
        speed2 = ((90 - angle) * 100) / angle;
        } else {
        speed2 = 100;
        speed1 = ((angle) * 100) / (90 - angle);
        }
        motorSpeed[0] = speed2;
        motorSpeed[1] = speed1;
        motorSpeed[2] = (speed2 * (-1));
        motorSpeed[3] = (speed1 * (-1));
        } else if (angle > 90 && angle <= 180) {
        angle = angle - 90;
        if (angle > (90 - angle)) {
        speed1 = 100;
        speed2 = ((90 - angle) * 100) / angle;
        } else {
         speed2 = 100;
         speed1 = ((angle) * 100) / (90 - angle);
         }
         motorSpeed[0] = (speed1 * (-1));
         motorSpeed[1] = (speed2);
         motorSpeed[2] = speed1;
         motorSpeed[3] = (speed2 * (-1));
         } else if (angle > 180 && angle <= 270) {
         angle = angle - 180;
         if (angle > (90 - angle)) {
          speed1 = 100;
         speed2 = ((90 - angle) * 100) / angle;
        } else {
         speed2 = 100;
         speed1 = ((angle) * 100) / (90 - angle);
         }
         motorSpeed[0] =  speed2 * (-1);
         motorSpeed[1] = (speed1 * (-1));
         motorSpeed[2] = speed2;
         motorSpeed[3] = speed1;
        } else {
         angle = angle - 270;
         if (angle > (90 - angle)) {
         speed1 = 100;
         speed2 = ((90 - angle) * 100) / angle;
         } else {
          speed2 = 100;
          speed1 = ((angle) * 100) / (90 - angle);
         }
         motorSpeed[0] =  speed1;
        motorSpeed[1] = (speed2 * (-1));
         motorSpeed[2] = ((speed1) * (-1));
          motorSpeed[3] = speed2;
    }

    //if(m_ourOrientation <= M_PI_2 || m_ourOrientation > 3* M_PI_2)
   // {
        if(m_ourOrientation <= M_PI_2 || m_ourOrientation > 3* M_PI_2)
        {
        entry->robotData.motor_left =  -motorSpeed[0];
        entry->robotData.motor_right = -motorSpeed[2];
        entry->robotData.motor_front = -motorSpeed[1];
        entry->robotData.motor_rear =  -motorSpeed[3];
        }
        else
        {
            entry->robotData.motor_left =  motorSpeed[0];
            entry->robotData.motor_right = motorSpeed[2];
            entry->robotData.motor_front = motorSpeed[1];
            entry->robotData.motor_rear =  motorSpeed[3];
        }

   // }
   /* else {
        if(old_angle<0)
        {
        entry->robotData.motor_left =  -motorSpeed[0];
        entry->robotData.motor_right =  motorSpeed[2];
        entry->robotData.motor_front =  -motorSpeed[1];
        entry->robotData.motor_rear =  motorSpeed[3];
        }
        else
        {
            entry->robotData.motor_left =  motorSpeed[0];
            entry->robotData.motor_right =  -motorSpeed[2];
            entry->robotData.motor_front =  motorSpeed[1];
            entry->robotData.motor_rear =  -motorSpeed[3];
    }
       }*/

    //entry->robotData.motor_front = 100;
   // entry->robotData.motor_rear =  100;
    //entry->robotData.motor_left = 100;
   // entry->robotData.motor_right = 100;

}
