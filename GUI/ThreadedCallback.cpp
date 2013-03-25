#include <SharedMem.h>
#include <Logging.h>

#include "ThreadedCallback.h"

CAICallback::CAICallback(CVisionComm *pVisionComm, AIControl *pAICtrl, CVisionMod  *pPlotter)
    : m_pVisionComm(pVisionComm)
    , m_pAICtrl(pAICtrl)
    , m_pPlotter(pPlotter)
{

}

void CAICallback::process()
{
    unsigned nextIdx = (sharedMem.aiIdx + 1)&SH_MEM_SIZE_MASK;
    TAIEntry *ai   = &sharedMem.AIdata[nextIdx];

    /// Frst check if we are operational

    if(!m_pVisionComm->IsConnected()){
        loggingObj->ShowCriticalError("AI Callback: NO CONNECTION TO VISION");
        return;
    }

    // 1. Read new coordinates from vision
    m_pVisionComm->ReadData(&ai->visionData);

    // 2. Read robot state
    /// TODO: read data from robot if needed

    // 3. Run AI to generate new set of points
    m_pAICtrl->RunAI(ai);

    // 4. Increment index
    sharedMem.aiIdx = nextIdx;

    // 5. Update plotter
    m_pPlotter->UpdateWindow();

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
    unsigned nextIdx = (sharedMem.navIdx + 1)&SH_MEM_SIZE_MASK;
    TNavEntry *nav = &sharedMem.NavData[nextIdx];

    /// Frst check if we are operational

    if(!m_pVisionComm->IsConnected()){
        loggingObj->ShowCriticalError("Nav Callback: NO CONNECTION TO VISION");
        return;
    }

    if(!m_pIBtComm->IsConnected()){
        loggingObj->ShowCriticalError("Nav Callback: NO CONNECTION TO ROBOT");
        return;
    }

    // 1. Read new coordinates from vision
    m_pVisionComm->ReadData(&nav->visionData);

    // 2. Read robot state
    /// TODO: read data from robot if needed

    // 3. Generate motor values
    m_pCNav->GenerateValues(nav);

    // 4. Send motor values to robot
    if(sharedMem.systemState != eStop)
        m_pIBtComm->SendData(&nav->robot.sendData);

    // 5. Increment index
    sharedMem.navIdx = nextIdx;

    emit finished();
}
