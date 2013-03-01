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
          //      entry->visionData.yellow_angle;
    }
    else{
        m_ourPos_x = (float) entry->visionData.blue_x;
        m_ourPos_y = (float) entry->visionData.blue_y;
        m_ourOrientation = entry->aiData.path[0].orientation;
        //        entry->visionData.blue_angle;
    }
    //m_ourOrientation = entry->aiData.path[0].orientation;
    memset(&entry->robotData, 0, sizeof(TRobotData));
    //GenerateLinear(entry);
    float dx;
    float dy;
    float theta;
    int  motorSpeed[4];
    float angle;
    float old_angle;
	int distToTarget;
   dx = (int)entry->aiData.path[1].position_X - (int)entry->aiData.path[0].position_X;
   dy = (int)entry->aiData.path[1].position_Y - (int)entry->aiData.path[0].position_Y;

	distToTarget = (int) sqrt((dx * dx)+(dy * dy));


    //make theta the direction to move in.
    if ((dx > 0) && (dy < 0)) {
		theta = atan((-dy)/dx);
	}
    else if ((dx < 0) && (dy < 0)) {
		theta = M_PI - atan(dy/dx);
	}

    else if ((dx < 0) && (dy >= 0)) {
		theta = atan((-dy)/dx) + M_PI;
	}

    else if ((dx > 0) && (dy >= 0)) {
		theta = 2*M_PI - atan(dy/dx);
	}
    else if ((dx == 0) && (dy >= 0)) {
		theta = 3*M_PI_2;
	}
    else if ((dx == 0) && (dy < 0)) {
        theta = M_PI_2;
	}
    else {
	theta = 0;
	}

    //set the forward and backwards speeds.
    int forwardspeed;
    int rightspeed;
    int robotspeed;
    int rotatespeed;

    robotspeed = 40;
  //  forwardspeed = cos(theta + m_ourOrientation + M_PI_2) * robotspeed;
  //  rightspeed = sin(theta + m_ourOrientation + M_PI_2) * robotspeed;

   forwardspeed = cos(theta - m_ourOrientation) * robotspeed;
   rightspeed = sin(theta - m_ourOrientation) * robotspeed;

 //   forwardspeed = cos(theta ) * robotspeed;
 //   rightspeed = sin(theta ) * robotspeed;

    //set the rotate speed

//need to be rewriten so it rotates to face direction AI wants not the ball!
float rotate_dir;
rotate_dir = theta - m_ourOrientation;

if (rotate_dir < 0) {
	rotate_dir = rotate_dir + 2*M_PI;
}

if ((rotate_dir > 0.2) && (rotate_dir <= M_PI)){
   	rotatespeed = -5;
}
else if ((rotate_dir > M_PI) && (rotate_dir <= (2*M_PI-0.2))){
   	rotatespeed = 5;
}
else {
	rotatespeed = 0;
}


	
    //set the motor speeds
        motorSpeed[0] = -forwardspeed + rotatespeed;
        motorSpeed[1] = -rightspeed + rotatespeed;
        motorSpeed[2] = forwardspeed + rotatespeed;
        motorSpeed[3] = rightspeed + rotatespeed;

//	if (distToTarget < 65){
//send kick command!
//entry->robotData.kicker =  1;
//	}
//else {
//entry->robotData.kicker =  0;
//}

  //Send the speeds
//     if(m_ourOrientation <= M_PI_2 || m_ourOrientation > 3* M_PI_2)
 //    {
       entry->robotData.motor_left =  motorSpeed[3];
       entry->robotData.motor_right =  motorSpeed[1];
       entry->robotData.motor_front =  motorSpeed[0];
       entry->robotData.motor_rear =  motorSpeed[2];
//    }
//    else
 //    {
 //        entry->robotData.motor_left =  -motorSpeed[0];
  //       entry->robotData.motor_right =  -motorSpeed[2];
  //       entry->robotData.motor_front =  -motorSpeed[1];
  //       entry->robotData.motor_rear =  -motorSpeed[3];
//     }


/*
    // if(m_ourOrientation <= M_PI_2 || m_ourOrientation > 3* M_PI_2)
    //generate angle difference and normalise in degrees
    angle = (theta - m_ourOrientation)- M_PI_2;
    //else
    //angle = M_PI + (theta - m_ourOrientation);
    //entry->aiData.path[1].orientation = angle;
    old_angle = angle;
     if(angle<0)
     {
         angle = -angle;
     }
        //entry->aiData.path[1].orientation = angle;
    angle = angle*180/M_PI;
    int speed1, speed2;
    if (angle <= 90) {
        if (angle > (90 - angle)) {
            speed1 = 100;
            speed2 = ((90 - angle) * 100) / angle;
        } else {
            speed2 = 100;
            speed1 = ((-angle) * 100) / (90 - angle);
        }
        motorSpeed[0] = speed2;
        motorSpeed[1] = speed1;
        motorSpeed[2] = (-speed2);
        motorSpeed[3] = (-speed1);
    } else if (angle > 90 && angle <= 180) {
        angle = angle - 90;
        if (angle > (90 - angle)) {
            speed1 = 100;
            speed2 = ((90 - angle) * 100) / angle;
        } else {
            speed2 = 100;
            speed1 = ((angle) * 100) / (90 - angle);
        }
        motorSpeed[0] = (-speed1);
        motorSpeed[1] = (speed2);
        motorSpeed[2] = speed1;
        motorSpeed[3] = (-speed2);
    } else if (angle > 180 && angle <= 270) {
        angle = angle - 180;
        if (angle > (90 - angle)) {
            speed1 = 100;
            speed2 = ((90 - angle) * 100) / angle;
        } else {
            speed2 = 100;
            speed1 = ((angle) * 100) / (90 - angle);
        }
        motorSpeed[0] =  speed2;
        motorSpeed[1] = (-speed1);
        motorSpeed[2] = -speed2;
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
        motorSpeed[1] = (-speed2);
        motorSpeed[2] = (-speed1);
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
            entry->robotData.motor_right =  motorSpeed[2];
            entry->robotData.motor_front =  motorSpeed[1];
            entry->robotData.motor_rear =  motorSpeed[3];
        }
        loggingObj->ShowMsg(QString("angle to target : %1")
                                .arg(angle)

                .toAscii()
                                .data());
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
