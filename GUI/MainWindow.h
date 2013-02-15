#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <ui_mainwindow.h>

#include "Comm/BTComm.h"
#include "Comm/VisionComm.h"
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
    void NavToBallSlot();
    void MoveWithBallSlot();
    void StopeMvmntSlot();

    void TeamSetup();
    void ConnToVision();
    void ConnToBT();

    void TimerCallBack();

private:
    void SetupGUI();
    void InitSytem();

    CBtComm        *m_btComm;
    CVisionComm    *m_visionComm;
    CNavigation     m_nav;

    AIControl       aiCtrl;

    QTimer         m_timer;
};


#endif // MAINWINDOW_H
