#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <ui_mainwindow.h>

#include "LoggingWidget.h"
#include "Comm/BTComm.h"

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

private:
    void SetupGUI();
    void InitSytem();

    CLoggingWidget *m_logWdgt;
    CBtComm        *m_btComm;
};


#endif // MAINWINDOW_H
