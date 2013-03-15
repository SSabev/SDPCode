#include "Navigation.h"


#include <Logging.h>
#include "QString"
#include <string.h>
#include <cmath>

#define _USE_MATH_DEFINES

#define MOVE_SPEED     (20)
#define ROT_SPEED	(10)

int abs(int val){
    if(val < 0) return -val;
    else        return val;
}


unsigned char _kickerCnt = 0;


CNavigation::CNavigation()
{

}


void CNavigation::GenerateValues()
{
    TEntry *entry = &sharedMem.positioning[sharedMem.currentIdx];
    m_ourOrientation = entry->aiData.path[0].orientation;
    //m_ourOrientation = entry->visionData.yellow_angle;

    loggingObj->ShowMsg(QString("ourOrientation: %1")
                            .arg( m_ourOrientation)
                            .toAscii()
                            .data());


    float dx;
    float dy;
    float theta;
    int  motorSpeed[4];
    int distToTarget;
    dx = (int)entry->aiData.path[1].position_X - (int)entry->aiData.path[0].position_X;
    dy = (int)entry->aiData.path[1].position_Y - (int)entry->aiData.path[0].position_Y;

 //  distToTarget = (int) sqrt((dx * dx)+(dy * dy));




   if(entry->aiData.doWeHaveBall == 0){
        int ax = (int)entry->visionData.ball_x - (int)entry->aiData.path[0].position_X;
        int ay =(int)entry->visionData.ball_y - (int)entry->aiData.path[0].position_Y;
        distToTarget = (int) sqrt((ax * ax)+(ay * ay));
   }
   else{
       distToTarget = 0;
   }

   loggingObj->ShowMsg(QString("distToBall: %1")
                          .arg(distToTarget)
                          .toAscii()
                          .data());


    //make theta the direction to move in.


    if ((dx > 0) && (dy >= 0)) {
        theta = atan(dy/dx);
    }
    else if ((dx < 0) && (dy >= 0)) {
        theta = M_PI - atan((-dy)/dx);
    }

    else if ((dx < 0) && (dy < 0)) {
        theta = atan(dy/dx) + M_PI;
    }

    else if ((dx > 0) && (dy < 0)) {
        theta = (2*M_PI - atan((-dy)/dx));
    }
    else if ((dx == 0) && (dy >= 0)) {
        theta = M_PI_2;
    }
    else if ((dx == 0) && (dy < 0)) {
        theta = 3*M_PI_2;
    }
    else {
    theta = 0;
    }
    loggingObj->ShowMsg(QString("theta: %1")
                        .arg(theta)
                        .toAscii()
                        .data());
    int forwardspeed;
    int rightspeed;
    int robotspeed;
    int rotatespeed;

    robotspeed = MOVE_SPEED;

    //set the rotate speed

    float rotate_dir;
 //   rotate_dir = entry->aiData.path[1].orientation - m_ourOrientation;

    if (entry->aiData.doWeHaveBall == 1){
        rotate_dir = entry->aiData.path[1].orientation - m_ourOrientation;
    }
    else
    {
        if (distToTarget < 75) {
                    rotate_dir = theta- m_ourOrientation;
        }
        else {
            rotate_dir = 0.0;

        }
    }
    rotate_dir = entry->aiData.path[1].orientation - m_ourOrientation;
   
    if (rotate_dir < 0){
        rotate_dir = rotate_dir + 2*M_PI;
    }

    if ((rotate_dir > 0.3) && (rotate_dir <= M_PI)){
        rotatespeed = -ROT_SPEED;
        
    }
    else if ((rotate_dir > M_PI) && (rotate_dir <= (2*M_PI-0.3))){
        rotatespeed = ROT_SPEED;
        
    }
    else {
        rotatespeed = 0;
    }


      
    loggingObj->ShowMsg(QString("rotateSpeed: %1")
                            .arg(rotatespeed)
                            .toAscii()
                            .data());

    //set the motor speeds
    
    int speed1, speed2;
     int angle = (theta- m_ourOrientation)*180/M_PI;
     //angle =90;
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


    //Decide whever to kick or not
	
    if(entry->aiData.doWeHaveBall == 1 && forwardspeed < 10){
        forwardspeed = 10;
        rightspeed = 0;
        //rotatespeed = rotatespeed/2;
     }


    if (_kickerCnt > 0)  _kickerCnt--;

    if ((entry->aiData.shouldKick == 1) && (_kickerCnt == 0)){
        //    send kick command!
        entry->robot.sendData.kicker =  1;
        _kickerCnt = 10;
    }
    else {
        entry->robot.sendData.kicker =  0;
    }

    


    
    motorSpeed[0] =  -motorSpeed[0]+rotatespeed;
    motorSpeed[1] =  -motorSpeed[1]+rotatespeed;
    motorSpeed[2] =  -motorSpeed[2]+rotatespeed;
    motorSpeed[3] =  -motorSpeed[3]+rotatespeed;
   
    //Send the speeds
    entry->robot.sendData.motor_left_speed =  abs(motorSpeed[3]/2);
    entry->robot.sendData.motor_right_speed =  abs(motorSpeed[1]/2);
    entry->robot.sendData.motor_front_speed =  abs(motorSpeed[0]/2);
    entry->robot.sendData.motor_rear_speed =  abs(motorSpeed[2]/2);

    entry->robot.sendData.motor_left_dir = motorSpeed[3] >= 0 ? 1 : 0;
    entry->robot.sendData.motor_right_dir = motorSpeed[1] >= 0 ? 1 : 0;
    entry->robot.sendData.motor_front_dir = motorSpeed[0] >= 0 ? 1 : 0;
    entry->robot.sendData.motor_rear_dir = motorSpeed[2] >= 0 ? 1 : 0;



    loggingObj->ShowMsg(QString("left: %1 %2 right: %3 %4 front: %5 %6 back: %7 %8")
                            .arg(entry->robot.sendData.motor_left_dir)
                            .arg(entry->robot.sendData.motor_left_speed)
                            .arg(entry->robot.sendData.motor_right_dir)
                            .arg(entry->robot.sendData.motor_right_speed)
                            .arg(entry->robot.sendData.motor_front_dir)
                            .arg(entry->robot.sendData.motor_front_speed)
                            .arg(entry->robot.sendData.motor_rear_dir)
                            .arg(entry->robot.sendData.motor_rear_speed)
                            .toAscii()
                            .data());
}


void CNavigation::GenerateStop()
{
    TEntry *entry = &sharedMem.positioning[sharedMem.currentIdx];

    entry->robot.sendData.motor_left_speed  =  0;
    entry->robot.sendData.motor_right_speed =  0;
    entry->robot.sendData.motor_front_speed =  0;
    entry->robot.sendData.motor_rear_speed  =  0;
    entry->robot.sendData.kicker            =  0;
}
void CNavigation::kickerP()
{
    TEntry *entry = &sharedMem.positioning[sharedMem.currentIdx];

    entry->robot.sendData.motor_left_speed  =  0;
    entry->robot.sendData.motor_right_speed =  0;
    entry->robot.sendData.motor_front_speed =  0;
    entry->robot.sendData.motor_rear_speed  =  0;
    entry->robot.sendData.kicker            =  1;




}
