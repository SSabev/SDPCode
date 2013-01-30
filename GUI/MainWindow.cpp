#include "MainWindow.h"
#include "../Shared/SharedMem.h"
#include "../Shared/Logging.h"

#include <string.h>

MainWindow::MainWindow()
{
    SetupGUI();
    InitSytem();
}

MainWindow::~MainWindow()
{

}

void MainWindow::MoveStraightSlot()
{
    sharedMem.systemState = eMoveStraight;

    sharedMem.Positioning[0].robotData.motor_1 = 1;
    m_btComm->SendData(&sharedMem.Positioning[0].robotData);
}

void MainWindow::PenaltySlot()
{
    sharedMem.systemState = eDoPenalty;

    sharedMem.Positioning[0].robotData.motor_1 = 2;
    m_btComm->SendData(&sharedMem.Positioning[0].robotData);
}

void MainWindow::StopeMvmntSlot()
{
    sharedMem.systemState = eStop;

    sharedMem.Positioning[0].robotData.motor_1 = 3;
    m_btComm->SendData(&sharedMem.Positioning[0].robotData);
}

void MainWindow::SetupGUI()
{
    setupUi(this);

    connect(moveStraightBtn, SIGNAL(clicked()), this, SLOT(MoveStraightSlot()));
    connect(penaltyBtn, SIGNAL(clicked()), this, SLOT(PenaltySlot()));
    connect(stopBtn, SIGNAL(clicked()), this, SLOT(StopeMvmntSlot()));

    m_logWdgt = new CLoggingWidget(this);
    m_logWdgt->show();

    connect(actionSet_Pitch_Side, SIGNAL(triggered()), this, SLOT(SetPitchSide()));
    connect(actionConnect_To_Vision, SIGNAL(triggered()), this, SLOT(ConnToVision()));
    connect(actionConnect_To_BT, SIGNAL(triggered()), this, SLOT(ConnToBT()));
}

void MainWindow::InitSytem()
{
    loggingObj = (ILogging *) m_logWdgt;

    // zero-out the shared memory
    memset(&sharedMem, 0, sizeof(TShMem));
    // set current state as IDLE
    sharedMem.systemState = eIDLE;

    m_btComm = new CBtComm(this);
    m_visionComm = new CVisionComm(this);

    loggingObj->ShowMsg("Configured...");
}

void MainWindow::ConnToBT()
{
    m_btComm->ConnectToBT();
}

void MainWindow::ConnToVision()
{
    m_visionComm->ConnectToVision();
}

void MainWindow::SetPitchSide()
{

}
