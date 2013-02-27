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
    void StopeMvmntSlot();

    void TeamSetup();
    void ConnToVision();
    void ConnToBT();

    void TimerCallBack();

private:
    void SetupGUI();
    void InitSytem();

    IBTComm        *mIBtComm;
    CVisionComm    *m_visionComm;
    CNavigation     m_nav;
    CVisionMod     *vision;

    AIControl       aiCtrl;

    QTimer          m_timer;
};


#endif // MAINWINDOW_H
