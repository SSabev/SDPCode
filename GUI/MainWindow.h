#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <ui_mainwindow.h>

#include "LoggingWidget.h"
#include "Comm/BTComm.h"
#include "Comm/VisionComm.h"

#include <QMainWindow>

class MainWindow
        : public QMainWindow
        , public Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private slots:
    void MoveStraightSlot();
    void PenaltySlot();
    void StopeMvmntSlot();

    void SetPitchSide();
    void ConnToVision();
    void ConnToBT();

private:
    void SetupGUI();
    void InitSytem();

    CLoggingWidget *m_logWdgt;
    CBtComm        *m_btComm;
    CVisionComm    *m_visionComm;
};


#endif // MAINWINDOW_H
