#ifndef SHAREDMEM_H
#define SHAREDMEM_H

#define SH_MEM_SIZE 8

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

    unsigned      r_sensor  : 1;
    unsigned      l_sensor  : 1;
    unsigned      have_ball : 1;
} __attribute__ ((packed))  TEntry;

typedef struct
{
    TEntry      Positioning[SH_MEM_SIZE];
    unsigned    CurrentIdx;               // index of last retrieved position
} TShMem;

extern TShMem sharedMem;

#endif // SHAREDMEM_H
