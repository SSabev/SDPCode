#ifndef SHAREDMEM_H
#define SHAREDMEM_H

#include <stdint.h>

#define SH_MEM_SIZE 8
#define SH_MEM_SIZE_MASK (SH_MEM_SIZE - 1)
#define MAX_PATH_SIZE 30

typedef enum {
    eIDLE,
    eMatch,
    eLeftPenalty,
    eRightPenalty,
    eStop = eIDLE,   // Stop is the same as IDLE

    eDribbleBall,    // Second milestone
    eNavToBall       // Second milestone
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
    // This indicates that the AI couldn't calculate a path.
    // Data in shared memory may be out of date or garbage.
    uint32_t      isFailedFrame    : 1;
} __attribute__ ((packed)) TAIData;

typedef struct{
    //! TODO: need to identify data that is required
    ///       for robot movement
    /// For testing (Milestone 1) it's size is 2 integers
    unsigned char motor_fl;
    unsigned char motor_fr;
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

typedef struct{
    unsigned char r_sensor  : 1;
    unsigned char l_sensor  : 1;
    unsigned char have_ball : 1;
} __attribute__ ((packed)) TRobotState;

typedef struct
{
    TVisionData     visionData;
    TRobotData      robotData;
    TRobotState     robotState;
    TAIData         aiData;
} __attribute__ ((packed))  TEntry;

typedef enum{
    eNonOperational = 0,
    eBTPresent      = 1,
    eVisionPresent  = (1 << 1),
    eOperational    = (eBTPresent | eVisionPresent)
} TSystemStatus;

typedef struct
{
    TSystemState  systemState;
    unsigned      currentIdx;               // index of active TEntry frame
    TEntry        positioning[SH_MEM_SIZE];
    TPitchSide    pitchSide;
    TTeamColor    teamColor;
    TPitchCfg     pitchCfg;

    TSystemStatus systemStatus;
} TShMem;

extern TShMem sharedMem;

#endif // SHAREDMEM_H
