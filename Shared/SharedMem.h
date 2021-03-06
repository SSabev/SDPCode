#ifndef SHAREDMEM_H
#define SHAREDMEM_H

#include <stdint.h>

#define SH_MEM_SIZE 8
#define SH_MEM_SIZE_MASK (SH_MEM_SIZE - 1)
#define MAX_PATH_SIZE 30

typedef enum {
    eIDLE,
    eMatch,         // Football match
    ePenaltyDefend,
    ePenaltyAttack,
    eLeftPenalty,
    eRightPenalty,
    eStop = eIDLE    // Stop is the same as IDLE

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

typedef struct {
    float position_X;
    float position_Y;
    float orientation;
} __attribute__ ((packed)) TAIWaypoint;

typedef struct{
    unsigned      pathLength;
    TAIWaypoint    path[MAX_PATH_SIZE];
    uint32_t      shouldKick    : 1;
    uint32_t      doWeHaveBall  : 1;
    uint32_t      isMovingToBall    : 1;
    // This indicates that the AI couldn't calculate a path.
    // Data in shared memory may be out of date or garbage.
    uint32_t      isFailedFrame    : 1;
} __attribute__ ((packed)) TAIData;

//! As specified in the Docs/BT_protocol.doc in git repo
/// Size: 5 bytes
typedef struct{
    unsigned char motor_left_speed  : 7;
    unsigned char motor_left_dir    : 1;

    unsigned char motor_right_speed : 7;
    unsigned char motor_right_dir   : 1;

    unsigned char motor_rear_speed  : 7;
    unsigned char motor_rear_dir    : 1;

    unsigned char motor_front_speed : 7;
    unsigned char motor_front_dir   : 1;

    unsigned char kicker            : 1;
    unsigned char spinners          : 1;
} __attribute__ ((packed)) TRobotData;

//! As specified in the Docs/BT_protocol.doc in git repo
/// Size: 1 byte
typedef struct{
    unsigned char f_l_sensor    : 1;
    unsigned char f_r_sensor    : 1;
    unsigned char back_sensor   : 1;
    unsigned char have_ball     : 1;
    unsigned char reserved      : 4;
} __attribute__ ((packed)) TRobotState;

// Wrapper structure for int alingnment
typedef struct{
    TRobotData  sendData;       /// 5 bytes
    TRobotState receiveData;    /// 1 byte
    int16_t     reserved;       /// 2 bytes
}  __attribute__ ((packed)) TRobot;

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
    TVisionData     visionData;
    TAIData         aiData;
    TRobotState     robotState;    /// 1 byte
    char            reserved[3];   /// 3 bytes - for int alignment
} __attribute__ ((packed))  TAIEntry;

typedef struct
{
    TVisionData     visionData;
    TRobot          robot;
} __attribute__ ((packed))  TNavEntry;

typedef enum{
    eNonOperational = 0,
    eBTConnected    = 1,
    eVisionPresent  = (1 << 1),
    eOperational    = (eBTConnected | eVisionPresent),
    eBTDisconnected = (~eBTConnected),
    eVisionMissing  = (~eVisionPresent)
} TSystemStatus;

typedef struct
{
    TSystemState  systemState;
    unsigned      aiIdx;               // index of the most recent & complete TAIEntry
    TAIEntry      AIdata[SH_MEM_SIZE];
    unsigned      navIdx;              // index of the most recent & complete TNavEntry
    TNavEntry     NavData[SH_MEM_SIZE];
    TPitchSide    pitchSide;
    TTeamColor    teamColor;
    TPitchCfg     pitchCfg;

    TSystemStatus systemStatus;

    float         speed_scale;
} TShMem;

extern TShMem sharedMem;

#endif // SHAREDMEM_H
