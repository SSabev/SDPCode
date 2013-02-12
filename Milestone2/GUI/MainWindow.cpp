#include "MainWindow.h"
#include "Tools/TeamCfgDlg.h"

#include <SharedMem.h>
#include <Logging.h>

#include <string.h>
#include <cmath>
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
    TEntry *entry = &sharedMem.positioning[sharedMem.currentIdx];
    m_visionComm->ReadData(&entry->visionData);

    sharedMem.systemState = eDribbleBall;

    m_logWdgt->ShowMsg(QString("yellow: x = %1 y = %2 angle = %3\n"
                               "blue:   x = %4 y = %5 angle = %6\n"
                               "ball:   x = %7 y = %8")
                       .arg(entry->visionData.yellow_x)
                       .arg(entry->visionData.yellow_y)
                       .arg(entry->visionData.yellow_angle)
                       .arg(entry->visionData.blue_x)
                       .arg(entry->visionData.blue_y)
                       .arg(entry->visionData.blue_angle)
                       .arg(entry->visionData.ball_x)
                       .arg(entry->visionData.ball_y));
}

void MainWindow::NavToBallSlot()
{
    sharedMem.systemState = eNavToBall;

    TEntry *entry = &sharedMem.positioning[sharedMem.currentIdx];
    m_visionComm->ReadData(&entry->visionData);

    m_logWdgt->ShowMsg(QString("ball: x = %1 y = %2 \n")
                       .arg(entry->visionData.ball_x)
                       .arg(entry->visionData.ball_y));

        aiCtrl.RunAI();
    for(int i =0;i<entry->aiData.pathLength;i++)
    {
        m_logWdgt->ShowMsg(QString("point %1: x = %2 y = %3 \n")
                           .arg(i)
                           .arg(entry->aiData.path[i].position_X)
                           .arg(entry->aiData.path[i].position_Y));
    }

    m_timer.start(TIMER_INTERVAL_MS);

    /*
    TEntry *entry = &sharedMem.positioning[sharedMem.currentIdx];

    m_visionComm->ReadData(&entry->visionData);
   // entry->aiData.path[0].position_X = entry->visionData.ball_x;
  //  entry->aiData.path[0].position_Y = entry->visionData.ball_y;

    m_nav.GenerateValues();

    m_btComm->SendData(&entry->robotData);
    */
}

void MainWindow::StopeMvmntSlot()
{
    sharedMem.systemState = eStop;
    m_timer.stop();
    TEntry *entry = &sharedMem.positioning[sharedMem.currentIdx];
    entry->robotData.motor_fr = 0;
    entry->robotData.motor_fl = 0;
    m_btComm->SendData(&sharedMem.positioning[
                            sharedMem.currentIdx]
                       .robotData);

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

    sharedMem.pitchCfg.pitchWidth = 560;
    sharedMem.pitchCfg.pitchHeight = 306;

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

    TEntry *entry = &sharedMem.positioning[sharedMem.currentIdx];




    if(entry->visionData.yellow_angle < 0) entry->visionData.yellow_angle = -entry->visionData.yellow_angle;
    else
    if(entry->visionData.yellow_angle > 0 &&entry->visionData.yellow_angle<M_PI)  entry->visionData.yellow_angle = -entry->visionData.yellow_angle;
    else entry->visionData.yellow_angle = 2*M_PI - entry->visionData.yellow_angle ;


    if(entry->visionData.blue_angle < 0) entry->visionData.blue_angle = -entry->visionData.blue_angle;
    else
    if(entry->visionData.blue_angle > 0 &&entry->visionData.blue_angle<M_PI)  entry->visionData.blue_angle = -entry->visionData.blue_angle;
    else entry->visionData.blue_angle = 2*M_PI - entry->visionData.blue_angle ;

    m_logWdgt->ShowMsg(QString("yellow: x = %1 y = %2 angle = %3\n"
                               "blue:   x = %4 y = %5 angle = %6\n"
                               "ball:   x = %7 y = %8")
                       .arg(entry->visionData.yellow_x)
                       .arg(entry->visionData.yellow_y)
                       .arg(entry->visionData.yellow_angle)
                       .arg(entry->visionData.blue_x)
                       .arg(entry->visionData.blue_y)
                       .arg(entry->visionData.blue_angle)
                       .arg(entry->visionData.ball_x)
                       .arg(entry->visionData.ball_y));
    aiCtrl.RunAI();
    //entry->aiData.path[1].position_X = entry->visionData.ball_x;
    //entry->aiData.path[1].position_Y = entry->visionData.ball_y;

    m_nav.GenerateValues();


    m_logWdgt->ShowMsg(QString("target: x = %1 y = %2 \n"
                                "next: x = %3 y = %4 \n"
                               "motor:   left = %5 right = %6")
                       .arg(entry->aiData.path[0].position_X)
                       .arg(entry->aiData.path[0].position_Y)
                       .arg(entry->aiData.path[1].position_X)
                       .arg(entry->aiData.path[1].position_Y)
                       .arg(entry->robotData.motor_fl)
                       .arg(entry->robotData.motor_fr));

    // Send new data to the robot
    m_btComm->SendData(&sharedMem.positioning[
                            sharedMem.currentIdx]
                       .robotData);

    // increment index
    sharedMem.currentIdx = (sharedMem.currentIdx+1) & SH_MEM_SIZE_MASK;
}
