#include "Navigation.h"


#include <Logging.h>
#include "QString"
#include <string.h>
#include <cmath>

#define _USE_MATH_DEFINES

#define MOVE_SPEED     (20)
#define ROT_SPEED	(25)
#define ROT_PENALTY_SPEED	(45)
#define MAX_SPEED	(120)

int abs(int val){
    if(val < 0) return -val;
    else        return val;
}


int _kickerCnt = 0;


CNavigation::CNavigation()
{

}

typedef struct{
    int front;
    int rear;
    int left;
    int right;
} Speeds;

int max(int x, int y){
    return x>y ? x:y;
}

int max_abs(int x, int y){
    return max (abs(x), abs(y));
}

Speeds limit_speeds(Speeds speeds,float coeficient){
    int max_speed = max ( max_abs(speeds.left,speeds.right), max_abs(speeds.front, speeds.rear));
    speeds.left   = ((float)speeds.left/max_speed*MAX_SPEED)/coeficient;
    speeds.right  = ((float)speeds.right/max_speed*MAX_SPEED)/coeficient;
    speeds.front  = ((float)speeds.front/max_speed*MAX_SPEED)/coeficient;
    speeds.rear   = ((float)speeds.rear/max_speed*MAX_SPEED)/coeficient;

    return speeds;
}

Speeds add_rotation(Speeds speeds, float theta){
    int rotatespeed;
    if(theta<0) theta = M_PI*2 + theta;

    if ((theta > 0.4) && (theta <= M_PI)){


        rotatespeed = (int)((float)-ROT_SPEED*theta/M_PI);

    }
    else if ((theta > M_PI) && (theta <= (2*M_PI-0.4))){
        rotatespeed = (int)((float)ROT_SPEED*theta/M_PI);

    }
    else {
        rotatespeed = 0;
    }

    speeds.left   =   speeds.left - rotatespeed;
    speeds.right  =   speeds.right - rotatespeed;
    speeds.front  =   speeds.front - rotatespeed;
    speeds.rear   =   speeds.rear - rotatespeed;

    return speeds;

}

Speeds find_speeds_deffend_left_right(float theta, float m_ourOrientation ){
    Speeds speeds;
    int speed1, speed2;
    int  motorSpeed[4];
     int angle = (int)((theta- m_ourOrientation)*180/M_PI);
     if(angle<0) angle = 360 +angle;
    //angle =0;
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

           loggingObj->ShowMsg(QString("robot angle %1 target angle %2")
                                   .arg(m_ourOrientation)
                                        .arg(theta)
                                   .toAscii()
                                   .data());


      speeds.left   =  0;// -motorSpeed[0]/100*MAX_SPEED;
      speeds.right  =  0;// -motorSpeed[2]/100*MAX_SPEED;
      speeds.front  =   -motorSpeed[3]/100*MAX_SPEED;
      speeds.rear   =   -motorSpeed[1]/100*MAX_SPEED;

      //speeds = add_rotation(speeds,theta- m_ourOrientation);

      speeds = limit_speeds(speeds,1);


    return speeds;
}

Speeds find_speeds_attack(float theta, float m_ourOrientation ){
    Speeds speeds;

     float ttheta = theta- m_ourOrientation;





        speeds.left   =   0;
        speeds.right  =   0;
        speeds.front  =   0;
        speeds.rear   =   0;

        int rotatespeed;
        if(ttheta<0) ttheta = M_PI*2 + ttheta;

        if ((ttheta > 0.4) && (ttheta <= M_PI)){


            rotatespeed = (int)((float)-ROT_PENALTY_SPEED);

        }
        else if ((ttheta > M_PI) && (ttheta <= (2*M_PI-0.4))){
            rotatespeed = (int)((float)ROT_PENALTY_SPEED);

        }
        else {
            rotatespeed = 0;
        }

        speeds.left   =    - rotatespeed;
        speeds.right  =    - rotatespeed;
        speeds.front  =    - rotatespeed;
        speeds.rear   =    - rotatespeed;

        return speeds;


      //speeds = add_rotation(speeds,theta- m_ourOrientation);

      //speeds = limit_speeds(speeds,1);


    //return speeds;
}

Speeds find_speeds(float theta,float target_theta, float m_ourOrientation ){
    Speeds speeds;
    int speed1, speed2;
    int  motorSpeed[4];
     int angle = (int)((theta- m_ourOrientation)*180/M_PI);
     if(angle<0) angle = 360 +angle;
    //angle =0;
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


      speeds.left   =   -motorSpeed[0]/100*MAX_SPEED;
      speeds.right  =   -motorSpeed[2]/100*MAX_SPEED;
      speeds.front  =   -motorSpeed[3]/100*MAX_SPEED;
      speeds.rear   =   -motorSpeed[1]/100*MAX_SPEED;

//        speeds.left   =   0;
//        speeds.right  =   0;
//        speeds.front  =   0;
//        speeds.rear   =   0;

//        loggingObj->ShowMsg(QString("rotate robot angle %1 rotate angle %2")
//                                .arg(theta- m_ourOrientation)
//                                     .arg(theta)
//                                .toAscii()
//                                .data());

      speeds = add_rotation(speeds,target_theta- m_ourOrientation);

      speeds = limit_speeds(speeds,1);


    return speeds;
}

void setCurrentPos(TNavEntry *entry, int &x, int &y, float &theta, TTeamColor teamColor){
    if (teamColor == eBlueTeam){
        x = entry->visionData.blue_x;
        y = entry->visionData.blue_y;
        theta = entry->visionData.blue_angle;
    }
    else{
        x = entry->visionData.yellow_x;
        y = entry->visionData.yellow_y;
        theta = entry->visionData.yellow_angle;
    }
}

// true if a new path has been generated by AI
bool CNavigation::isNewPath(TAIEntry *ai){
    return (ai->aiData.pathLength != lastPathLen ||
            ai->aiData.path[0].position_X != lastCurrentX ||
            ai->aiData.path[0].position_Y != lastCurrentY);
}

void CNavigation::savePathState(TAIEntry *ai){
    lastPathLen = ai->aiData.pathLength;
    lastCurrentX = ai->aiData.path[0].position_X;
    lastCurrentY = ai->aiData.path[0].position_Y;
}

bool isAtTarget(int rx, int ry, float r_theta, int target_x, int target_y, float target_theta ){
    float dx = rx - target_x;
    float dy = ry - target_y;
    float d_theta = 60 * (r_theta - target_theta);
    float total_dist = dx*dx + dy*dy + d_theta*d_theta;

    return (total_dist < 30);
}


void CNavigation::GenerateValues(TNavEntry *entry)
{
    TAIEntry *ai = &sharedMem.AIdata[sharedMem.aiIdx];
    TTeamColor teamColor = sharedMem.teamColor;

    int x,y,target_x,target_y;
    float r_theta, target_theta;

    setCurrentPos(entry, x, y, m_ourOrientation,  teamColor);


    //m_ourOrientation = ai->aiData.path[0].orientation;


    float dx;
    float dy;
    float theta;
   // int distToTarget;




    if (isNewPath(ai)){
            targetId = 1;

            target_x = (int)ai->aiData.path[targetId].position_X;
            target_y = (int)ai->aiData.path[targetId].position_Y;
            target_theta = ai->aiData.path[targetId].orientation;
            //target_theta = standardize_angle(target_theta);
        }
    else
        {
            //Path is the same as last time, move to the next waypoint if needed



           while(isAtTarget(x, y, m_ourOrientation, target_x, target_y, target_theta ))
                if (targetId < ai->aiData.pathLength - 1 )
                    targetId++;

            target_x = (int)ai->aiData.path[targetId].position_X;
            target_y = (int)ai->aiData.path[targetId].position_Y;
            target_theta = ai->aiData.path[targetId].orientation;
            //target_theta = standardize_angle(target_theta);

        }


    dx = (int)target_x - x;
    dy = (int)target_y - y;

 //  distToTarget = (int) sqrt((dx * dx)+(dy * dy));




//   if(ai->aiData.doWeHaveBall == 0){
//        int ax = (int)entry->visionData.ball_x - (int)ai->aiData.path[0].position_X;
//        int ay =(int)entry->visionData.ball_y - (int)ai->aiData.path[0].position_Y;
//        distToTarget = (int) sqrt((ax * ax)+(ay * ay));
//   }
//   else{
//       distToTarget = 0;
//   }



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


       //set the rotate speed


    if (_kickerCnt >= 0)  _kickerCnt--;

    if ((ai->aiData.shouldKick == 1) && (_kickerCnt <= 0)){
        //    send kick command!
        entry->robot.sendData.kicker =  1;
        _kickerCnt = 15;
    }

    else entry->robot.sendData.kicker =  0;

    Speeds speeds;

    if (sharedMem.systemState == ePenaltyAttack)
    {

        speeds = find_speeds_attack(target_theta, m_ourOrientation);
        if(_kickerCnt <= 0)
        {
            entry->robot.sendData.kicker            =  1;
            _kickerCnt = 20;
        }
    }
    else if((sharedMem.systemState == ePenaltyDefend ))
    {
        speeds = find_speeds_deffend_left_right(theta, m_ourOrientation);
    }
    else
    {
        speeds = find_speeds(theta,target_theta, m_ourOrientation);

    //m_ourOrientation=0;
   //target_theta = M_PI/4;
   //Speeds speeds = find_speeds(target_theta, m_ourOrientation);


   //if we have the ball reduce our speed by half
//   if(ai->aiData.doWeHaveBall == 1){
//    speeds = limit_speeds(speeds,2);
//   }
    }

//   loggingObj->ShowMsg(QString("robot angle %1 target angle %2")
//                           .arg(m_ourOrientation)
//                                .arg(target_theta)
//                           .toAscii()
//                           .data());

    //Send the speeds

#if defined(NXT_BUILD)
    entry->robot.sendData.motor_left_speed =  abs(speeds.rear);
    entry->robot.sendData.motor_right_speed =  abs(speeds.front);
    entry->robot.sendData.motor_front_speed =  abs(speeds.right);
    entry->robot.sendData.motor_rear_speed =  abs(speeds.left);

    entry->robot.sendData.motor_left_dir = speeds.rear >= 0 ? 1 : 0;
    entry->robot.sendData.motor_right_dir = speeds.front >= 0 ? 1 : 0;

    entry->robot.sendData.motor_front_dir = speeds.right >= 0 ? 1 : 0;
    entry->robot.sendData.motor_rear_dir = speeds.left >= 0 ? 1 : 0;
#elif defined(ARDUINO_BLD)
    entry->robot.sendData.motor_left_speed =  abs(speeds.left);
    entry->robot.sendData.motor_right_speed =  abs(speeds.right);
    entry->robot.sendData.motor_front_speed =  abs(speeds.front);
    entry->robot.sendData.motor_rear_speed =  abs(speeds.rear);

    entry->robot.sendData.motor_left_dir = speeds.left >= 0 ? 1 : 0;
    entry->robot.sendData.motor_right_dir = speeds.right >= 0 ? 0 : 1;

    entry->robot.sendData.motor_front_dir = speeds.front >= 0 ? 0 : 1;
    entry->robot.sendData.motor_rear_dir = speeds.rear >= 0 ? 1 : 0;
#endif

    loggingObj->ShowMsg(QString("left: %1 %2 right: %3 %4 front: %5 %6 back: %7 %8 kick %9 %10")
                            .arg(entry->robot.sendData.motor_left_dir)
                            .arg(entry->robot.sendData.motor_left_speed)
                            .arg(entry->robot.sendData.motor_right_dir)
                            .arg(entry->robot.sendData.motor_right_speed)
                            .arg(entry->robot.sendData.motor_front_dir)
                            .arg(entry->robot.sendData.motor_front_speed)
                            .arg(entry->robot.sendData.motor_rear_dir)
                            .arg(entry->robot.sendData.motor_rear_speed)
                        .arg(entry->robot.sendData.kicker)
                        .arg( ai->aiData.shouldKick)

                            .toAscii()
                           .data());
}


void CNavigation::GenerateStop(TNavEntry *entry)
{
   // TEntry *entry = &sharedMem.positioning[sharedMem.currentIdx];

    entry->robot.sendData.motor_left_speed  =  0;
    entry->robot.sendData.motor_right_speed =  0;
    entry->robot.sendData.motor_front_speed =  0;
    entry->robot.sendData.motor_rear_speed  =  0;
    entry->robot.sendData.kicker            =  0;

    entry->robot.sendData.motor_left_dir = 0;
    entry->robot.sendData.motor_right_dir = 0;
    entry->robot.sendData.motor_front_dir = 0;
    entry->robot.sendData.motor_rear_dir = 0;

}
void CNavigation::kickerP(TNavEntry *entry)
{
    //TEntry *entry = &sharedMem.positioning[sharedMem.currentIdx];

    entry->robot.sendData.motor_left_speed  =  0;
    entry->robot.sendData.motor_right_speed =  0;
    entry->robot.sendData.motor_front_speed =  0;
    entry->robot.sendData.motor_rear_speed  =  0;
    entry->robot.sendData.kicker            =  1;




}
void CNavigation::PenaltyDefend(TNavEntry *entry)
{
    entry->robot.sendData.motor_left_speed  =  0;
    entry->robot.sendData.motor_right_speed =  0;
    entry->robot.sendData.motor_front_speed =  0;
    entry->robot.sendData.motor_rear_speed  =  0;
    entry->robot.sendData.kicker            =  0;
}

void CNavigation::PenaltyAttack(TNavEntry *entry)
{
    entry->robot.sendData.motor_left_speed  =  0;
    entry->robot.sendData.motor_right_speed =  0;
    entry->robot.sendData.motor_front_speed =  0;
    entry->robot.sendData.motor_rear_speed  =  0;
    entry->robot.sendData.kicker            =  0;
}
