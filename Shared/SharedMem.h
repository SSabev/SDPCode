#ifndef SHAREDMEM_H
#define SHAREDMEM_H

#include <stdint.h>

#define SH_MEM_SIZE 8

typedef enum {
    eIDLE,
    eMatch,
    eLeftPenalty,
    eRightPenalty,
    eStop = eIDLE,   // Same as IDLE

    eMoveStraight,   // First milestone
    eDoPenalty       // First milestone
} TSystemState;

typedef enum{
    eLeftSide,
    eRightSide
} TPitchSide;


typedef enum{
    eYellowTeam,
    eBlueTeam
} TTeamColor;

typedef struct{
    unsigned pitchWidth;
    unsigned pitchHeight;
} __attribute__ ((packed)) TPitchCfg;

typedef struct{
    //! TODO: need to identify data that is required
    ///       for robot movement
    /// For testing (Milestone 1) it's size is 2 integers
    unsigned char motor_1;
    unsigned char motor_2;
    unsigned char motor_3;
    unsigned char motor_4;
    uint32_t      kicker    : 1;
} __attribute__ ((packed)) TRobotData;

typedef struct{
    unsigned    yellow_x;
    unsigned    yellow_y;
    float       yellow_angle;

    unsigned    blue_x;
    unsigned    blue_y;
    float       blue_angle;

    unsigned    ball_x;
    unsigned    ball_y;

    unsigned    timestamp;
} __attribute__ ((packed)) TVisionData;


typedef struct
{
    TVisionData visionData;
    TRobotData robotData;

    // next field to be recieved from robot
    unsigned      r_sensor  : 1;
    unsigned      l_sensor  : 1;
    unsigned      have_ball : 1;
} __attribute__ ((packed))  TEntry;

typedef struct
{
    TSystemState  systemState;
    unsigned      currentIdx;               // index of active TEntry frame
    TEntry        positioning[SH_MEM_SIZE];
    TPitchSide    pitchSide;
    TTeamColor    teamColor;
    TPitchCfg     pitchCfg;
} TShMem;

extern TShMem sharedMem;

#endif // SHAREDMEM_H
