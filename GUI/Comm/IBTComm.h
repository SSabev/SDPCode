#ifndef IBTCOMM_H
#define IBTCOMM_H

#include <SharedMem.h>

class IBTComm
{
public:
    virtual void ConnectToRobot() = 0;

    virtual bool SendData(TRobotData *data) = 0;
    virtual bool ReadData(TRobotState *data) = 0;
};


#endif // IBTCOMM_H
