#ifndef THREADEDCALLBACK_H
#define THREADEDCALLBACK_H

#include <QObject>

#include <AIControl.h>
#include "Navigation/Navigation.h"

#include "Comm/IBTComm.h"
#include "Comm/VisionComm.h"

#include "Tools/CVisionMod.h"

class CAICallback
        : public QObject
{
    Q_OBJECT

public:
    CAICallback(CVisionComm *pVisionComm, AIControl *pAICtrl);

public slots:
    void process();

signals:
    void finished();
    void UpdatePlotter();

private:
    CVisionComm *m_pVisionComm;
    AIControl   *m_pAICtrl;
};


class CNavCallback
        : public QObject
{
    Q_OBJECT

public:
    CNavCallback(CVisionComm *pVisionComm, IBTComm *pIBtComm, CNavigation *pCNav);

public slots:
    void process();

signals:
    void finished();

private:
    CVisionComm *m_pVisionComm;
    IBTComm     *m_pIBtComm;
    CNavigation *m_pCNav;
};


#endif // THREADEDCALLBACK_H
