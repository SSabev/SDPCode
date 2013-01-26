#ifndef SHAREDMEM_H
#define SHAREDMEM_H

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

typedef struct{
    //! TODO: need to identify data that is required
    ///       for robot movement
    unsigned char motor_1;
    unsigned char motor_2;
    unsigned char motor_3;
    unsigned char motor_4;
    unsigned      kicker    : 1;
} __attribute__ ((packed)) TRobotData;

typedef struct
{
    unsigned char yellow_x;
    unsigned char yellow_y;
    unsigned char yellow_angle;

    unsigned char blue_x;
    unsigned char blue_y;
    unsigned char blue_angle;

    unsigned char ball_x;
    unsigned char ball_y;

    unsigned      timestamp;

    // next 5 fields to be sent out to robot

    TRobotData robotData;

    // next field to be recieved from robot
    unsigned      r_sensor  : 1;
    unsigned      l_sensor  : 1;
    unsigned      have_ball : 1;
} __attribute__ ((packed))  TEntry;

typedef struct
{
    TSystemState  systemState;
    unsigned      CurrentIdx;               // index of active TEntry frame
    TEntry        Positioning[SH_MEM_SIZE];
} TShMem;

extern TShMem sharedMem;

#endif // SHAREDMEM_H
