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
    void StopMvmntSlot();
    void StartMatch();
    void DoPenalty();
    void DefendPenalty();

    void TeamSetup();
    void ConnToVision();
    void ConnToBT();
    void UpdatePlotter();


private:
    void SetupGUI();
    void InitSytem();

    void StartProcess(TSystemState state);
    void StartThreads();

    IBTComm        *m_pIBtComm;
    CVisionComm    *m_pVisionComm;
    CNavigation     m_nav;
    CVisionMod     *vision;

    AIControl       aiCtrl;

    QThread        *m_pAIThread;
    QThread        *m_pNavThread;
};


#endif // MAINWINDOW_H
