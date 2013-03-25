#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <ui_mainwindow.h>

#include "Comm/IBTComm.h"
#include "Comm/VisionComm.h"
#include "Tools/CVisionMod.h"

#include "Navigation/Navigation.h"

#include <AIControl.h>

#include <QMainWindow>
#include <QTimer>
#include <QThread>
#include <QMutex>

class MainWindow
        : public QMainWindow
        , public Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private slots:
    void Action1Slot();
    void Action2Slot();
    void StopMvmntSlot();

    void TeamSetup();
    void ConnToVision();
    void ConnToBT();

//    void TimerCallBack();
    void NavTimerCallback();
    void AIStage1Callback();
    void AIStage2Callback();

private:
    void SetupGUI();
    void InitSytem();

    IBTComm        *m_pIBtComm;
    CVisionComm    *m_pVisionComm;
    CNavigation     m_nav;
    CVisionMod     *vision;

    AIControl       aiCtrl;

    QTimer          m_AI_Timer;
    QTimer          m_Nav_Timer;

    QThread        *m_pAIThread;
    QThread        *m_pNavThread;
};


#endif // MAINWINDOW_H
