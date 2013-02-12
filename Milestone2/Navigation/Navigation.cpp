//#include "Navigation.h"


//#include <Logging.h>

//#include <string.h>
//#include <cmath>

//#define _USE_MATH_DEFINES

///// These values for 100ms and for 1st milestone type robot
//#define MAX_DISPLACEMENT_PX     (3.0)
//#define MAX_DISPLACEMENT_PX_SQ  (MAX_DISPLACEMENT_PX * MAX_DISPLACEMENT_PX)
//#define ANGULAR_THRESHOLD       (M_PI_4) // 45 degrees

//#define MAX_ONLY_ANGULAR_MSPEED (50)
//#define MAX_ALGULAR_VEL_100ms   (0.5)
//#define MAX_FORWARD_MSPEED      (40)
//#define MAX_ANGULAR_MSPEED      (30)
//#define ANGULAR_RATIO           (MAX_ANGULAR_MSPEED / MAX_ALGULAR_VEL_100ms)

//#define ROBOT_D                 (13) // cm
//#define WEEL_D                  (3)  // cm
//#define SPEED_COEFFICIENT       (2*ROBOT_D*10/WEEL_D)
//#define MAX_SPEED               (300)
//#define SCALING_FACTOR          (10)

//typedef enum{
//    eTurnLeft,
//    eTurnRight
//} TTurnDir;

//CNavigation::CNavigation()
//{
//    memset(&m_target, 0, sizeof(TTarget));
//}

//void CNavigation::GenerateValues()
//{
//    TEntry *entry = &sharedMem.positioning[sharedMem.currentIdx];

//    if(sharedMem.teamColor == eYellowTeam){
//        m_ourPos_x = (float) entry->visionData.yellow_x;
//        m_ourPos_y = (float) entry->visionData.yellow_y;
//        m_ourOrientation = entry->visionData.yellow_angle;
//    }
//    else{
//        m_ourPos_x = (float) entry->visionData.blue_x;
//        m_ourPos_y = (float) entry->visionData.blue_y;
//        m_ourOrientation = entry->visionData.blue_angle;
//    }

//    int dx;
//    int dy;
//    float theta;


//    dx = (int)entry->aiData.path[1].position_X - m_ourPos_x;
//    dy = (int)entry->aiData.path[1].position_Y - m_ourPos_y;

//    theta = atan2(dy,dx);
//    entry->aiData.path[1].orientation = theta - m_ourOrientation;
//    entry->aiData.path[0].orientation = theta;

//    if((entry->aiData.path[1].orientation>0 && entry->aiData.path[3].orientation<0)||(entry->aiData.path[1].orientation<0 && entry->aiData.path[3].orientation>0))
//             entry->aiData.path[1].orientation =  entry->aiData.path[3].orientation;
//            memset(&entry->robotData, 0, sizeof(TRobotData));
//   // GenerateLinear(entry);


//    // check if we have to rotate before we can move straight
//    if(std::abs(m_ourOrientation - entry->aiData.path[1].orientation) > ANGULAR_THRESHOLD){
//        GenerateMaxAngular(entry);
//    }
//    else{
//        GenerateLinear(entry);
//       // GenerateAngular(entry);
//    }
//    entry->aiData.path[3].orientation =theta - m_ourOrientation;


//}

//void CNavigation::GenerateMaxAngular(TEntry *entry)
//{
//    if((m_ourOrientation - entry->aiData.path[1].orientation) >0 ){
//        // turn right
//        entry->robotData.motor_fl = MAX_ONLY_ANGULAR_MSPEED;
//        entry->robotData.motor_fr = -MAX_ONLY_ANGULAR_MSPEED;
//    }
//    else{
//        // turn left
//        entry->robotData.motor_fl = -MAX_ONLY_ANGULAR_MSPEED;
//        entry->robotData.motor_fr = MAX_ONLY_ANGULAR_MSPEED;
//    }
//}

//void CNavigation::GenerateLinear(TEntry *entry)
//{
//    TTurnDir turnDir;
//    int dOmega;
//    float theta;
//    if (theta - m_ourOrientation > 0)
//        turnDir = eTurnLeft;
//    else
//        turnDir = eTurnRight;

//    dOmega = int (theta * (float)SPEED_COEFFICIENT);

//    if (eTurnLeft){
//        entry->robotData.motor_fl = (MAX_SPEED - dOmega) / SCALING_FACTOR;
//        entry->robotData.motor_fr = MAX_SPEED;
//    }
//    else{
//        entry->robotData.motor_fr = (MAX_SPEED - dOmega) / SCALING_FACTOR;
//        entry->robotData.motor_fl = MAX_SPEED;
//    }

//    /*
//    float ourPos_x;
//    float ourPos_y;
//    float distToPoint_sq;
//    float pathPoint_x = entry->aiData.path[0].position_X;
//    float pathPoint_y = entry->aiData.path[0].position_Y;

//    TVisionData *vision = &entry->visionData;
//    TRobotData *motors = &entry->robotData;

//    if(sharedMem.teamColor == eYellowTeam){
//        ourPos_x = (float) vision->yellow_x;
//        ourPos_x = (float) vision->yellow_y;
//    }
//    else{
//        ourPos_x = (float) vision->blue_x;
//        ourPos_x = (float) vision->blue_y;
//    }

//    distToPoint_sq = (pathPoint_x - ourPos_x)*(pathPoint_x - ourPos_x)
//            + (pathPoint_y - ourPos_y)*(pathPoint_y - ourPos_y);
//    if(distToPoint_sq > MAX_DISPLACEMENT_PX_SQ){
//        // path point is too far away - we can't reach it in between next
//        // interrupt => set max linear speed to the motors

//    }
//    */
//    //entry->robotData.motor_fr = MAX_SPEED;
//    //entry->robotData.motor_fl = MAX_SPEED;
//}

//void CNavigation::GenerateAngular(TEntry *entry)
//{
//    float diff = m_ourOrientation - entry->aiData.path[1].orientation;

//    if(std::abs(diff) > MAX_ALGULAR_VEL_100ms){
//        if(diff < 0){
//            // turn right
//            entry->robotData.motor_fl += MAX_ANGULAR_MSPEED;
//            entry->robotData.motor_fr -= MAX_ANGULAR_MSPEED;
//        }
//        else{
//            // turn left
//            entry->robotData.motor_fl -= MAX_ANGULAR_MSPEED;
//            entry->robotData.motor_fr += MAX_ANGULAR_MSPEED;
//        }
//    }
//    else{
//        entry->robotData.motor_fl += diff*ANGULAR_RATIO;
//        entry->robotData.motor_fr -= diff*ANGULAR_RATIO;
//    }
//}


#include "Navigation.h"

#include <QString>


#include <Logging.h>

#include <string.h>
#include <cmath>

#define _USE_MATH_DEFINES

/// These values for 100ms and for 1st milestone type robot
#define MAX_DISPLACEMENT_PX     (3.0)
#define MAX_DISPLACEMENT_PX_SQ  (MAX_DISPLACEMENT_PX * MAX_DISPLACEMENT_PX)
#define ANGULAR_THRESHOLD       (M_PI_4) // 45 degrees

#define MAX_ONLY_ANGULAR_MSPEED (30)
#define MAX_ALGULAR_VEL_100ms   (0.12)
#define MAX_FORWARD_MSPEED      (50)
#define MAX_ANGULAR_MSPEED      (10)
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

static float prevOrientation;

CNavigation::CNavigation()
{
    memset(&m_target, 0, sizeof(TTarget));
}

void CNavigation::GenerateMStone2()
{
    int dx;
    int dy;
    float theta;
    TTurnDir turnDir;
    int dOmega;

    TEntry *entry = &sharedMem.positioning[sharedMem.currentIdx];

    if(sharedMem.teamColor == eYellowTeam){
        m_ourPos_x = (float) entry->visionData.yellow_x;
        m_ourPos_y = (float) entry->visionData.yellow_y;
        m_ourOrientation = entry->visionData.yellow_angle;
    }
    else{
        m_ourPos_x = (float) entry->visionData.blue_x;
        m_ourPos_y = (float) entry->visionData.blue_y;
        m_ourOrientation = entry->visionData.blue_angle;
    }

    memset(&entry->robotData, 0, sizeof(TRobotData));

    if((entry->aiData.path[1].orientation>0 && prevOrientation<0)||(entry->aiData.path[1].orientation<0 && prevOrientation>0))
        entry->aiData.path[1].orientation =  prevOrientation;

    dx = (int)entry->aiData.path[1].position_X - m_ourPos_x;
    dy = (int)entry->aiData.path[1].position_Y - m_ourPos_y;
    theta = atan2(dy,dx);

    if (theta - m_ourOrientation > 0)
        turnDir = eTurnLeft;
    else
        turnDir = eTurnRight;

        dOmega = int (theta * (float)SPEED_COEFFICIENT);

    if (eTurnLeft){
        entry->robotData.motor_fl = (MAX_SPEED - dOmega) / SCALING_FACTOR;
        entry->robotData.motor_fr = MAX_SPEED;
    }
    else{
        entry->robotData.motor_fr = (MAX_SPEED - dOmega) / SCALING_FACTOR;
        entry->robotData.motor_fl = MAX_SPEED;
    }
}

void CNavigation::GenerateValues()
{
    TEntry *entry = &sharedMem.positioning[sharedMem.currentIdx];

    if(sharedMem.teamColor == eYellowTeam){
        m_ourPos_x = (float) entry->visionData.yellow_x;
        m_ourPos_y = (float) entry->visionData.yellow_y;
        m_ourOrientation = entry->visionData.yellow_angle;
    }
    else{
        m_ourPos_x = (float) entry->visionData.blue_x;
        m_ourPos_y = (float) entry->visionData.blue_y;
        m_ourOrientation = entry->visionData.blue_angle;
    }

    memset(&entry->robotData, 0, sizeof(TRobotData));

    if((entry->aiData.path[1].orientation>0 && prevOrientation<0)||(entry->aiData.path[1].orientation<0 && prevOrientation>0))
        entry->aiData.path[1].orientation =  prevOrientation;
    int dx;
    int dy;
    float theta;


    dx = (int)entry->aiData.path[1].position_X - m_ourPos_x;
    dy = (int)entry->aiData.path[1].position_Y - m_ourPos_y;

    theta = atan2(dy,dx);
    entry->aiData.path[1].orientation = theta - m_ourOrientation;
    entry->aiData.path[0].orientation = theta;


    if(std::abs(entry->aiData.path[1].orientation) > ANGULAR_THRESHOLD){
        loggingObj->ShowMsg("Just rotate");
        GenerateMaxAngular(entry);
    }
    else{
        loggingObj->ShowMsg("Move with rotation");
        GenerateLinear(entry);
        GenerateAngular(entry);
    }
    prevOrientation = theta - m_ourOrientation;
}

void CNavigation::GenerateMaxAngular(TEntry *entry)
{   //rotate the wrong way so that nav work for most cases
    if((true))
    {
    if((entry->aiData.path[1].orientation) > 0){
        // turn right
        loggingObj->ShowMsg("Just rotate right");
        entry->robotData.motor_fl = MAX_ONLY_ANGULAR_MSPEED;
        entry->robotData.motor_fr = -MAX_ONLY_ANGULAR_MSPEED;
    }
    else{
        // turn left
        loggingObj->ShowMsg("Just rotate left");
        entry->robotData.motor_fl = -MAX_ONLY_ANGULAR_MSPEED;
        entry->robotData.motor_fr = MAX_ONLY_ANGULAR_MSPEED;
    }
    }
    /*else
    {
        if((m_ourOrientation - entry->aiData.path[0].orientation) > 0){
            // turn right
            loggingObj->ShowMsg("Just rotate right");
            entry->robotData.motor_fl = MAX_ONLY_ANGULAR_MSPEED;
            entry->robotData.motor_fr = -MAX_ONLY_ANGULAR_MSPEED;
        }
        else{
            // turn left
            loggingObj->ShowMsg("Just rotate left");
            entry->robotData.motor_fl = -MAX_ONLY_ANGULAR_MSPEED;
            entry->robotData.motor_fr = MAX_ONLY_ANGULAR_MSPEED;
        }
    }*/
    }


void CNavigation::GenerateLinear(TEntry *entry)
{
    /*
    float ourPos_x;
    float ourPos_y;
    float distToPoint_sq;
    float pathPoint_x = entry->aiData.path[0].position_X;
    float pathPoint_y = entry->aiData.path[0].position_Y;

    TVisionData *vision = &entry->visionData;
    TRobotData *motors = &entry->robotData;

    if(sharedMem.teamColor == eYellowTeam){
        ourPos_x = (float) vision->yellow_x;
        ourPos_x = (float) vision->yellow_y;
    }
    else{
        ourPos_x = (float) vision->blue_x;
        ourPos_x = (float) vision->blue_y;
    }

    distToPoint_sq = (pathPoint_x - ourPos_x)*(pathPoint_x - ourPos_x)
            + (pathPoint_y - ourPos_y)*(pathPoint_y - ourPos_y);
    if(distToPoint_sq > MAX_DISPLACEMENT_PX_SQ){
        // path point is too far away - we can't reach it in between next
        // interrupt => set max linear speed to the motors

    }
    */
    entry->robotData.motor_fl = MAX_FORWARD_MSPEED;
    entry->robotData.motor_fr = MAX_FORWARD_MSPEED;
}

void CNavigation::GenerateAngular(TEntry *entry)
{
    float diff = entry->aiData.path[1].orientation;

    if(std::abs(diff) > MAX_ALGULAR_VEL_100ms){
        if(diff > 0){
            // turn right
            loggingObj->ShowMsg("Turn right max");
            entry->robotData.motor_fl += MAX_ANGULAR_MSPEED;
            entry->robotData.motor_fr -= MAX_ANGULAR_MSPEED;
        }
        else{
            // turn left
            loggingObj->ShowMsg("Turn left max");
            entry->robotData.motor_fl -= MAX_ANGULAR_MSPEED;
            entry->robotData.motor_fr += MAX_ANGULAR_MSPEED;
        }
    }
    else{
        if(diff >0)
            loggingObj->ShowMsg("Turn right");
        else
            loggingObj->ShowMsg("Turn left");

        entry->robotData.motor_fl += diff*ANGULAR_RATIO;
        entry->robotData.motor_fr -= diff*ANGULAR_RATIO;
    }
}
