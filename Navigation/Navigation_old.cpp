#include "Navigation.h"


#include <Logging.h>
#include "QString"
#include <string.h>
#include <cmath>
#include <time.h>

#define _USE_MATH_DEFINES

#define MOVE_SPEED     (60)
#define ROT_SPEED	   (5)

int abs(int val){
    if(val < 0) return -val;
    else        return val;
}


unsigned char _kickerCnt = 0;


CNavigation::CNavigation()
{

}

/*void CNavigation::GenerateValues()
{
    TEntry *entry;

    float dx;
    float dy;
    float theta;
    float dtheta;
    int vf; // front velocity
    int vs; // sideways velocity

    int front;8
    int rear;
    int left;
    int right;

    entry = &sharedMem.positioning[sharedMem.currentIdx];

    dx = (entry->aiData.path[1].position_X - entry->aiData.path[0].position_X);
    dy = (entry->aiData.path[1].position_Y - entry->aiData.path[0].position_Y);

    theta = dy/dx;
    vf = MOVE_SPEED * cos(theta);
    vs = MOVE_SPEED * sin(theta);

    dtheta = entry->aiData.path[1].orientation - entry->aiData.path[0].orientation;
    if(abs((int)dtheta) > 0.2) {
        if(dtheta > 0){
            front = vs  + ROT_SPEED;
            rear  = -vs + ROT_SPEED;

            left  = vf  + ROT_SPEED;
            right = -vf + ROT_SPEED;
        }
        else{
            front = vs  - ROT_SPEED;
            rear  = -vs - ROT_SPEED;

            left  = vf  - ROT_SPEED;
            right = -vf - ROT_SPEED;
        }
    }

    //Send the speeds
    entry->robot.sendData.motor_left_speed =  abs(left);
    entry->robot.sendData.motor_right_speed = abs(right);
    entry->robot.sendData.motor_front_speed = abs(front);
    entry->robot.sendData.motor_rear_speed =  abs(rear);

    entry->robot.sendData.motor_front_dir = front >= 0 ? 1 : 0;
    entry->robot.sendData.motor_rear_dir  = rear  >= 0 ? 1 : 0;
    entry->robot.sendData.motor_left_dir  = left  >= 0 ? 1 : 0;
    entry->robot.sendData.motor_right_dir = right >= 0 ? 1 : 0;
}*/


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
 /*   loggingObj->ShowMsg(QString("theta: %1")
                        .arg(theta)
                        .toAscii()
                        .data());*/
    int forwardspeed;
    int rightspeed;
    int robotspeed;
    int rotatespeed;

    if (entry->aiData.doWeHaveBall == 0){
    robotspeed = MOVE_SPEED;
    }
    else{
        robotspeed =  30;
    }

  //  forwardspeed = cos(theta + m_ourOrientation + M_PI_2) * robotspeed;
  //  rightspeed = sin(theta + m_ourOrientation + M_PI_2) * robotspeed;

   forwardspeed = cos(theta - m_ourOrientation) * robotspeed;
   rightspeed = - sin(theta - m_ourOrientation) * robotspeed;




    //set the rotate speed

    float rotate_dir;
    rotate_dir = entry->aiData.path[1].orientation - m_ourOrientation;

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

/*    while (rotate_dir < 0)
    {
        rotate_dir += 2*M_PI;
    }

    rotate_dir = fmod(rotate_dir, 2*M_PI);*/

    if (rotate_dir < 0){
        rotate_dir = rotate_dir + 2*M_PI;
    }

    if ((rotate_dir > 0.2) && (rotate_dir <= M_PI)){
        rotatespeed = -ROT_SPEED;
     //   forwardspeed = 0;
     //   rightspeed = 0;
    }
    else if ((rotate_dir > M_PI) && (rotate_dir <= (2*M_PI-0.2))){
        rotatespeed = ROT_SPEED;
    //    forwardspeed = 0;
    //    rightspeed = 0;
    }
    else {
        rotatespeed = 0;
    }


    if(entry->aiData.doWeHaveBall == 1 && forwardspeed < 10){
        forwardspeed = 30;
        rightspeed = 0;
        //rotatespeed = rotatespeed/2;
     }

    loggingObj->ShowMsg(QString("forwardSpeed: %1")
                            .arg(forwardspeed)
                            .toAscii()
                            .data());

    loggingObj->ShowMsg(QString("rightSpeed: %1")
                            .arg(rightspeed)
                            .toAscii()
                            .data());

    loggingObj->ShowMsg(QString("rotateSpeed: %1")
                            .arg(rotatespeed)
                            .toAscii()
                            .data());



    //set the motor speeds
    motorSpeed[0] = -forwardspeed + rotatespeed;
    motorSpeed[1] = rightspeed + rotatespeed;
    motorSpeed[2] = forwardspeed + rotatespeed;
    motorSpeed[3] = -rightspeed + rotatespeed;



    //Decide whever to kick or not

    if (_kickerCnt > 0)  _kickerCnt--;

    if ((entry->aiData.shouldKick == 1) && (_kickerCnt == 0)){
        //    send kick command!
        entry->robot.sendData.kicker =  1;
        _kickerCnt = 10;
    }
    else {
        entry->robot.sendData.kicker =  0;
    }

    //Send the speeds
    entry->robot.sendData.motor_left_speed =  abs(motorSpeed[3]);
    entry->robot.sendData.motor_right_speed =  abs(motorSpeed[1]);
    entry->robot.sendData.motor_front_speed =  abs(motorSpeed[0]);
    entry->robot.sendData.motor_rear_speed =  abs(motorSpeed[2]);

    entry->robot.sendData.motor_front_dir = motorSpeed[0] >= 0 ? 1 : 0;
    entry->robot.sendData.motor_rear_dir = motorSpeed[2] >= 0 ? 1 : 0;
    entry->robot.sendData.motor_left_dir = motorSpeed[3] >= 0 ? 1 : 0;
    entry->robot.sendData.motor_right_dir = motorSpeed[1] >= 0 ? 1 : 0;
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

/*void CNavigation::kickerP()
{
    TEntry *entry = &sharedMem.positioning[sharedMem.currentIdx];

    entry->robot.sendData.motor_left_speed  =  0;
    entry->robot.sendData.motor_right_speed =  0;
    entry->robot.sendData.motor_front_speed =  0;
    entry->robot.sendData.motor_rear_speed  =  0;
    entry->robot.sendData.kicker            =  1;




}
*/
