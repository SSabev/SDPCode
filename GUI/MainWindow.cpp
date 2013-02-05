#include "MainWindow.h"
#include "Tools/TeamCfgDlg.h"

#include <SharedMem.h>
#include <Logging.h>

#include <string.h>

#define TIMER_INTERVAL_MS 20

MainWindow::MainWindow()
    : m_timer(this)
{
    SetupGUI();
    InitSytem();
}

MainWindow::~MainWindow()
{

}

void MainWindow::MoveWithBallSlot()
{
    sharedMem.systemState = eDribbleBall;
}

void MainWindow::NavToBallSlot()
{
    sharedMem.systemState = eNavToBall;
}

void MainWindow::StopeMvmntSlot()
{
    sharedMem.systemState = eStop;
}

void MainWindow::SetupGUI()
{
    setupUi(this);

    connect(moveWithBallBtn, SIGNAL(clicked()), this, SLOT(MoveWithBallSlot()));
    connect(NavToBallBtn, SIGNAL(clicked()), this, SLOT(NavToBallSlot()));
    connect(stopBtn, SIGNAL(clicked()), this, SLOT(StopeMvmntSlot()));

    m_logWdgt = new CLoggingWidget(this);
    m_logWdgt->show();

    connect(connToVisionBtn, SIGNAL(clicked()), this, SLOT(ConnToVision()));
    connect(btConnectBtn, SIGNAL(clicked()), this, SLOT(ConnToBT()));
    connect(teamSetupBtn, SIGNAL(clicked()), this, SLOT(TeamSetup()));
    connect(logBtn, SIGNAL(clicked()), this, SLOT(ShowLogWin()));

    m_timer.setSingleShot(false);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(TimerCallBack()));
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

    sharedMem.currentIdx = 0;

    aiCtrl.Initialise();

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

void MainWindow::TeamSetup()
{
    CTeamCfgDlg dlg(this);
    dlg.exec();
}

void MainWindow::ShowLogWin()
{
    m_logWdgt->show();
}

void MainWindow::TimerCallBack()
{
    if(!sharedMem.systemStatus == eOperational){
        return;
    }

    // Read Modules new information
    m_btComm->ReadData(
                &sharedMem.positioning[
                    sharedMem.currentIdx]
                .robotState);
    m_visionComm->ReadData(&sharedMem.positioning[
                                sharedMem.currentIdx]
                           .visionData);

    /// TODO: Call AI

    /// TODO: Call Navigation

    // Send new data to the robot
    m_btComm->SendData(&sharedMem.positioning[
                            sharedMem.currentIdx]
                       .robotData);

    // increment index
    sharedMem.currentIdx = (sharedMem.currentIdx+1) & SH_MEM_SIZE_MASK;
}
