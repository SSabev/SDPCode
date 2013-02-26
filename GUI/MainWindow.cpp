#include "MainWindow.h"
#include "Tools/TeamCfgDlg.h"

#include <SharedMem.h>
#include <Logging.h>

#include <string.h>

#ifdef ARDUINO_BLD
#include "Comm/ArduinoComm.h"
#endif
#ifdef NXT_BUILD
#include "Comm/BTComm.h"
#endif
#if (!defined ARDUINO_BLD && !defined NXT_BUILD) || (defined ARDUINO_BLD && defined NXT_BUILD)
#error Must specify which type of communicator to use
#endif

#define TIMER_INTERVAL_MS 100

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
//    TEntry *entry = &sharedMem.positioning[sharedMem.currentIdx];
//    entry->aiData.path[0].position_X = 2;
//    entry->aiData.path[0].position_Y = 3;
//    entry->aiData.path[1].position_X = 100;
//    entry->aiData.path[1].position_Y = 100;
//    entry->aiData.path[2].position_X = 200;
//    entry->aiData.path[2].position_Y = 300;
//    entry->aiData.path[3].position_X = 500;
//    entry->aiData.path[3].position_Y = 350;

//    entry->aiData.pathLength = 4;

//    vision->UpdateWindow();
}

void MainWindow::NavToBallSlot()
{
    /*
    sharedMem.systemState = eNavToBall;
    TEntry *entry = &sharedMem.positioning[sharedMem.currentIdx];

    m_visionComm->ReadData(&entry->visionData);
    entry->aiData.path[0].position_X = entry->visionData.ball_x;
    entry->aiData.path[0].position_Y = entry->visionData.ball_y;

    m_nav.GenerateValues();

    m_btComm->SendData(&entry->robotData);
    */
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

    connect(connToVisionBtn, SIGNAL(clicked()), this, SLOT(ConnToVision()));
    connect(btConnectBtn, SIGNAL(clicked()), this, SLOT(ConnToBT()));
    connect(teamSetupBtn, SIGNAL(clicked()), this, SLOT(TeamSetup()));

    m_timer.setSingleShot(false);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(TimerCallBack()));
}

void MainWindow::InitSytem()
{
    loggingObj = (ILogging *) LoggingWidget;

    // zero-out the shared memory
    memset(&sharedMem, 0, sizeof(TShMem));
    // set current state as IDLE
    sharedMem.systemState = eIDLE;

//    m_btComm = new CBtComm(this);
#ifdef ARDUINO_BLD
    CArduinoComm *btComm = new CArduinoComm(this);
    mIBtComm = (IBTComm *) btComm;
#endif
#ifdef NXT_BUILD
    CBtComm *btComm = new CBtComm(this);
    mIBtComm = (IBTComm *) btComm;
#endif

    m_visionComm = new CVisionComm(this);

    sharedMem.currentIdx = 0;

    aiCtrl.Initialise();

    vision = new CVisionMod(this);
    vision->show();

    loggingObj->ShowMsg("Configured...");
}

void MainWindow::ConnToBT()
{
    mIBtComm->ConnectToRobot();
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

void MainWindow::TimerCallBack()
{
    if(!sharedMem.systemStatus == eOperational){
        return;
    }
    /*

    // Read Modules new information
    m_btComm->ReadData(
                &sharedMem.positioning[
                    sharedMem.currentIdx]
                .robotState);
    m_visionComm->ReadData(&sharedMem.positioning[
                                sharedMem.currentIdx]
                           .visionData);

    aiCtrl.RunAI();

    m_nav.GenerateValues();

    // Send new data to the robot
    m_btComm->SendData(&sharedMem.positioning[
                            sharedMem.currentIdx]
                       .robotData);

    // increment index
    sharedMem.currentIdx = (sharedMem.currentIdx+1) & SH_MEM_SIZE_MASK;
    */
}
