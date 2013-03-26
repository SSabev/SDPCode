#include "Navigation.h"


#include <Logging.h>
#include "QString"
#include <string.h>
#include <cmath>
#include <time.h>

#define _USE_MATH_DEFINES

//#define MOVE_SPEED     (80)
//#define ROT_SPEED	   (5)


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

#define KP 0.5
#define KI 0
#define KD 0
#define MAX_SPEED 120
#define MIN_SPEED 110
#define HAVE_BALL_SPEED 90
#define MIN_TURN_ANGLE 0.2
#define ROTATE_PERCENT 50


int pid(int error, int integral, int derivative)
{
    int temp =  KP*error + KI*integral + KD*derivative;
    return temp;
}

float standardize_angle(float angle)
{   
    //assume the angle is out-of-phrase by at most 2pi
    if (angle < 0) angle = angle + 2*M_PI;
    if (angle > 2*M_PI) angle = angle - 2*M_PI;
    return angle;
}

//get p percent of abs(speed)
int get_percent(int speed, float p)
{
    speed = abs(speed);
    return (int) speed * (p*1.0 / 100);

}


typedef struct{
    int front;
    int rear;
    int left;
    int right;
} Speeds;


// add rotation to the speeds. The robot should rotate faster when the target is closer
Speeds add_rotation(Speeds s, float t_theta, int dist){
    float rot_amount;
    if (dist >0)
        rot_amount = 1000.0/dist;
    else
        rot_amount = 1000;

    if (t_theta < M_PI)
    {
        //turn left
        rot_amount = rot_amount * (t_theta/M_PI);
        s.front = (int) s.front + rot_amount;
        s.right = (int) s.right + rot_amount;

        s.rear =  (int) s.rear - rot_amount;
        s.left =  (int) s.left - rot_amount;
    }
    else
    {
        //turn right
        rot_amount = rot_amount * (2*M_PI - t_theta)/M_PI;
        s.front = (int) s.front - rot_amount;
        s.right = (int) s.right - rot_amount;

        s.rear =  (int) s.rear + rot_amount;
        s.left =  (int)s.left + rot_amount;
    }
    
    
    loggingObj->ShowMsg(QString("rot_amount for rotate %1")
                            .arg( rot_amount )
                            .toAscii()
                            .data());
    return s;
}


int max(int x, int y){
    return x>y ? x:y;
}

int max_abs(int x, int y){
    return max (abs(x), abs(y));
}

// fix the speeds so that the max speed is at (MIN_SPEED, MAX_SPEED)
Speeds limit_speed(Speeds s){
    TAIEntry *ai = &sharedMem.AIdata[sharedMem.aiIdx];
    int max_speed = max ( max_abs(s.left,s.right), max_abs(s.front, s.rear));
    if (max_speed == 0) return s;
    float ratio = 1;


    if (max_speed < MIN_SPEED){
        ratio = MIN_SPEED*1.0/max_speed;
    }

    if (max_speed > MAX_SPEED){
        ratio = MAX_SPEED*1.0/max_speed;
    }

    if (ai->aiData.doWeHaveBall == 1){
        ratio = HAVE_BALL_SPEED *1.0/max_speed;
    }

    //apply the ratio
    s.left = (int) s.left * ratio;
    s.right = (int) s.right * ratio;
    s.front = (int) s.front * ratio;
    s.rear = (int) s.rear * ratio;

    return s;
}




// Args: current (x,y,theta) and target (x,y,theta)
Speeds find_speeds(int rx, int ry, float r_theta, int target_x, int target_y, float target_theta ){
    Speeds res;
    //target pos wrt the robot
    int tx = target_x - rx;
    int ty = target_y - ry;
    float t_theta = standardize_angle(target_theta - r_theta);

    // rotate the point about r_theta
    // |  cos(theta)   sin(theta) |
    // | -sin(theta)   cos(theta) |

    int temp_x = tx* cos(r_theta) + ty*sin(r_theta);
    int temp_y = tx*-sin(r_theta) + ty*cos(r_theta);

    tx = temp_x;
    ty = temp_y;

    // at this point, (tx, ty, t_theta) is the coordinates of the target in the robot's system


    res.left = res.right = pid (tx, 0, 0);
    res.front = res.rear = pid (ty, 0, 0);

    //res = limit_speed(res);

    //if (t_theta > MIN_TURN_ANGLE && t_theta < 2*M_PI - MIN_TURN_ANGLE)
   // {
        res = add_rotation(res, t_theta, sqrt(tx*tx + ty*ty) );
        loggingObj->ShowMsg(QString("rotating %1")
                                .arg( t_theta )
                                .toAscii()
                                .data());
    //}

    res = limit_speed(res);

    return res;
}

//set current position of the robot from vision data
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
//    TEntry *entry = &sharedMem.positioning[sharedMem.currentIdx];
    TAIEntry *ai = &sharedMem.AIdata[sharedMem.aiIdx];
    TTeamColor teamColor = sharedMem.teamColor;


    m_ourOrientation = ai->aiData.path[0].orientation;
    //m_ourOrientation = entry->visionData.yellow_angle;

    loggingObj->ShowMsg(QString("ourOrientation: %1")
                            .arg( m_ourOrientation)
                            .toAscii()
                            .data());



    int distToTarget;
    //dx = (int)entry->aiData.path[1].position_X - (int)entry->aiData.path[0].position_X;
    //dy = (int)entry->aiData.path[1].position_Y - (int)entry->aiData.path[0].position_Y;
    
    int rx,ry,target_x,target_y;
    float r_theta, target_theta;
    
    //current robot position
    //rx = (int)ai->aiData.path[0].position_X;
    //ry = (int)ai->aiData.path[0].position_Y;
    //r_theta = m_ourOrientation;

    setCurrentPos(entry, rx, ry, r_theta,  teamColor);
    
    //target position
    if (isNewPath(ai)){
        targetId = 1;

    }
    else
    {
        //Path is the same as last time, move to the next waypoint if needed

        target_x = (int)ai->aiData.path[targetId].position_X;
        target_y = (int)ai->aiData.path[targetId].position_Y;
        target_theta = ai->aiData.path[targetId].orientation;
        target_theta = standardize_angle(target_theta);

        if (isAtTarget(rx, ry, r_theta, target_x, target_y, target_theta ))
            if (targetId < ai->aiData.pathLength - 1 )
                targetId++;

    }

    savePathState(ai);

    target_x = (int)ai->aiData.path[targetId].position_X;
    target_y = (int)ai->aiData.path[targetId].position_Y;
    target_theta = ai->aiData.path[targetId].orientation;
    target_theta = standardize_angle(target_theta);


    // go to the ball instead
    //target_x = (int)entry->visionData.ball_x;
    //target_y = (int)entry->visionData.ball_y;
    //target_theta = r_theta;


    Speeds res = find_speeds(rx, ry, r_theta, target_x, target_y, target_theta);

    //keep the ball
    if(ai->aiData.doWeHaveBall == 1){
        //if (res.left < 15) res.left = 15;
        //if (res.right < 15) res.right = 15;

         res.left =  res.left;
         res.right =  res.right;
         res.rear = res.rear ;
         res.front = res.front;
    }


    //dis to target
   if(ai->aiData.doWeHaveBall == 0){
        int ax = (int)entry->visionData.ball_x - (int)ai->aiData.path[0].position_X;
        int ay = (int)entry->visionData.ball_y - (int)ai->aiData.path[0].position_Y;
        distToTarget = (int) sqrt((ax * ax)+(ay * ay));
   }
   else{
       distToTarget = 0;
   }



   loggingObj->ShowMsg(QString("distToBall: %1")
                          .arg(distToTarget)
                          .toAscii()
                          .data());


    //Decide whever to kick or not

    if (_kickerCnt > 0)  _kickerCnt--;

    if ((ai->aiData.shouldKick == 1) && (_kickerCnt == 0)){
        //    send kick command!
        entry->robot.sendData.kicker =  1;
        _kickerCnt = 10;
    }
    else {
        entry->robot.sendData.kicker =  0;
    }
    
    //res.rear = 100;
    //res.front = res.right = res.left = 100;

    //Send the speeds


    if(ai->aiData.pathLength == 1)
    {

        entry->robot.sendData.motor_left_speed =  0;
        entry->robot.sendData.motor_right_speed =  0;
        entry->robot.sendData.motor_front_speed =  0;
        entry->robot.sendData.motor_rear_speed =  0;

        loggingObj->ShowMsg(QString("current rx = %1").arg(rx).toAscii().data());
        loggingObj->ShowMsg(QString("current ry = %1").arg(ry).toAscii().data());
        loggingObj->ShowMsg(QString("current target_x = %1").arg(target_x).toAscii().data());
        loggingObj->ShowMsg(QString("current target_y = %1").arg(target_y).toAscii().data());

        loggingObj->ShowMsg(QString("left %1").arg(res.left).toAscii().data());
        loggingObj->ShowMsg(QString("right %1").arg(res.right).toAscii().data());
        loggingObj->ShowMsg(QString("front %1").arg(res.front).toAscii().data());
        loggingObj->ShowMsg(QString("rear %1").arg(res.rear).toAscii().data());



        entry->robot.sendData.motor_left_dir = 0;
        entry->robot.sendData.motor_right_dir = 0;

       entry->robot.sendData.motor_front_dir = 0;
        entry->robot.sendData.motor_rear_dir = 0;
     }
    else
    {
//        entry->robot.sendData.motor_left_speed =  abs(res.rear);
//        entry->robot.sendData.motor_right_speed =  abs(res.front);
//        entry->robot.sendData.motor_front_speed =  abs(res.right);
//        entry->robot.sendData.motor_rear_speed =  abs(res.left);

//        loggingObj->ShowMsg(QString("current rx = %1").arg(rx).toAscii().data());
//        loggingObj->ShowMsg(QString("current ry = %1").arg(ry).toAscii().data());
//        loggingObj->ShowMsg(QString("current target_x = %1").arg(target_x).toAscii().data());
//        loggingObj->ShowMsg(QString("current target_y = %1").arg(target_y).toAscii().data());

//        loggingObj->ShowMsg(QString("left %1").arg(res.left).toAscii().data());
//        loggingObj->ShowMsg(QString("right %1").arg(res.right).toAscii().data());
//        loggingObj->ShowMsg(QString("front %1").arg(res.front).toAscii().data());
//        loggingObj->ShowMsg(QString("rear %1").arg(res.rear).toAscii().data());



//        entry->robot.sendData.motor_left_dir = res.rear >= 0 ? 1 : 0;
//        entry->robot.sendData.motor_right_dir = res.front >= 0 ? 0 : 1;

//       entry->robot.sendData.motor_front_dir = res.right >= 0 ? 0 : 1;
//        entry->robot.sendData.motor_rear_dir = res.left >= 0 ? 1 : 0;
//}
    
    
    entry->robot.sendData.motor_left_dir = res.left >= 0 ? 1 : 0;
    entry->robot.sendData.motor_right_dir = res.right >= 0 ? 0 : 1;
    
    entry->robot.sendData.motor_front_dir = res.front >= 0 ? 0 : 1;
    entry->robot.sendData.motor_rear_dir = res.rear >= 0 ? 1 : 0;
    
    entry->robot.sendData.motor_left_speed =  abs(res.left);
    entry->robot.sendData.motor_right_speed =  abs(res.right);
    entry->robot.sendData.motor_front_speed =  abs(res.front);
    entry->robot.sendData.motor_rear_speed =  abs(res.rear);

    loggingObj->ShowMsg(QString("current rx = %1").arg(rx).toAscii().data());
    loggingObj->ShowMsg(QString("current ry = %1").arg(ry).toAscii().data());
    loggingObj->ShowMsg(QString("current target_x = %1").arg(target_x).toAscii().data());
    loggingObj->ShowMsg(QString("current target_y = %1").arg(target_y).toAscii().data());

    loggingObj->ShowMsg(QString("left %1").arg(res.left).toAscii().data());
    loggingObj->ShowMsg(QString("right %1").arg(res.right).toAscii().data());
    loggingObj->ShowMsg(QString("front %1").arg(res.front).toAscii().data());
    loggingObj->ShowMsg(QString("rear %1").arg(res.rear).toAscii().data());



    entry->robot.sendData.motor_left_dir = res.left >= 0 ? 1 : 0;
    entry->robot.sendData.motor_right_dir = res.right >= 0 ? 0 : 1;

    entry->robot.sendData.motor_front_dir = res.front >= 0 ? 0 : 1;
    entry->robot.sendData.motor_rear_dir = res.rear >= 0 ? 1 : 0;
}

}


void CNavigation::GenerateStop(TNavEntry *entry)
{
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
    entry->robot.sendData.motor_left_speed  =  0;
    entry->robot.sendData.motor_right_speed =  0;
    entry->robot.sendData.motor_front_speed =  0;
    entry->robot.sendData.motor_rear_speed  =  0;
    entry->robot.sendData.kicker            =  1;
}
