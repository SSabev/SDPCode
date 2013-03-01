#include "Navigation.h"


#include <Logging.h>
#include "QString"
#include <string.h>
#include <cmath>

#define _USE_MATH_DEFINES

#define MoveSpeed     (40)
#define RotSpeed	  (5)

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
    float dx;
    float dy;
    float theta;
    int  motorSpeed[4];
//    int distToTarget;
    dx = (int)entry->aiData.path[1].position_X - (int)entry->aiData.path[0].position_X;
    dy = (int)entry->aiData.path[1].position_Y - (int)entry->aiData.path[0].position_Y;

//    distToTarget = (int) sqrt((dx * dx)+(dy * dy));


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

    robotspeed = MoveSpeed;

    forwardspeed = cos(theta - m_ourOrientation) * robotspeed;
    rightspeed = sin(theta - m_ourOrientation) * robotspeed;


    //set the rotate speed

    float rotate_dir;
    rotate_dir = entry->aiData.path[1].orientation - m_ourOrientation;

    if (rotate_dir < 0) {
        rotate_dir = rotate_dir + 2*M_PI;
    }

    if ((rotate_dir > 0.2) && (rotate_dir <= M_PI)){
        rotatespeed = -RotSpeed;
    }
    else if ((rotate_dir > M_PI) && (rotate_dir <= (2*M_PI-0.2))){
        rotatespeed = RotSpeed;
    }
    else {
        rotatespeed = 0;
    }



    //set the motor speeds
    motorSpeed[0] = -forwardspeed + rotatespeed;
    motorSpeed[1] = -rightspeed + rotatespeed;
    motorSpeed[2] = forwardspeed + rotatespeed;
    motorSpeed[3] = rightspeed + rotatespeed;

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
