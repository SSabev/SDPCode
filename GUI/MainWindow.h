#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <ui_mainwindow.h>

#include "Logging.h"

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

    CLogging *m_logWdgt;
};


#endif // MAINWINDOW_H
