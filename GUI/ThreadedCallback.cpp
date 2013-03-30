#include <SharedMem.h>
#include <Logging.h>

#include <time.h>
#include <stdio.h>

#include "ThreadedCallback.h"

CAICallback::CAICallback(CVisionComm *pVisionComm, AIControl *pAICtrl)
    : m_pVisionComm(pVisionComm)
    , m_pAICtrl(pAICtrl)
{

}

void CAICallback::process()
{
    unsigned nextIdx;
    TAIEntry *ai;
    struct timespec tim;

    tim.tv_sec = 0;
    tim.tv_nsec = 50000000L; // 50 ms

    while(true){
        nanosleep(&tim, NULL);

        if(sharedMem.systemState == eStop)
            break;

        nextIdx = (sharedMem.aiIdx + 1)&SH_MEM_SIZE_MASK;
        ai   = &sharedMem.AIdata[nextIdx];

        /// Frst check if we are operational

        if(!m_pVisionComm->IsConnected()){
            loggingObj->ShowCriticalError("AI Callback: NO CONNECTION TO VISION");
            return;
        }

        // 1. Read new coordinates from vision
        m_pVisionComm->ReadData(&ai->visionData);

        // 2. Read robot state
        /// TODO: read data from robot if needed

#ifdef DRY_RUN
        printf("AI cycle\n");
#else
        // 3. Run AI to generate new set of points
        m_pAICtrl->RunAI(ai);

        // 4. Increment index
        sharedMem.aiIdx = nextIdx;

        emit UpdatePlotter();
#endif
    }

    emit finished();
}

CNavCallback::CNavCallback(CVisionComm *pVisionComm, IBTComm *pIBtComm, CNavigation *pCNav)
    : m_pVisionComm(pVisionComm)
    , m_pIBtComm(pIBtComm)
    , m_pCNav(pCNav)
{

}

void CNavCallback::process()
{
    unsigned nextIdx;
    TNavEntry *nav;
    struct timespec tim;

    tim.tv_sec = 0;
    tim.tv_nsec = 50000000L; // 50 ms

    while(true){
        nanosleep(&tim, NULL);

        nextIdx = (sharedMem.navIdx + 1)&SH_MEM_SIZE_MASK;
        nav = &sharedMem.NavData[nextIdx];

        /// Frst check if we are operational

        if(!m_pVisionComm->IsConnected()){
            loggingObj->ShowCriticalError("Nav Callback: NO CONNECTION TO VISION");
            if(sharedMem.systemState == eStop){
                loggingObj->ShowCriticalError("Nav Callback terminated");
                break;
            }
            else
                continue;
        }

        if(!m_pIBtComm->IsConnected()){
            loggingObj->ShowCriticalError("Nav Callback: NO CONNECTION TO ROBOT");
            if(sharedMem.systemState == eStop){
                loggingObj->ShowCriticalError("Nav Callback terminated");
                break;
            }
            else
                continue;
        }

        // 1. Read new coordinates from vision
        m_pVisionComm->ReadData(&nav->visionData);

        // 2. Read robot state
        /// TODO: read data from robot if needed

#ifdef DRY_RUN
        printf("NAV cycle\n");
#else
        // 3. Generate motor values
        if(sharedMem.systemState == eStop)
            m_pCNav->GenerateStop(nav);
        else
            m_pCNav->GenerateValues(nav);
#endif
        // 4. Send motor values to robot
        m_pIBtComm->SendData(&nav->robot.sendData);

        // 5. Increment index
        sharedMem.navIdx = nextIdx;

        if(sharedMem.systemState == eStop)
            break;
    }

    emit finished();
}
