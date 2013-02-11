#include "MainWindow.h"
#include "Tools/TeamCfgDlg.h"

#include <SharedMem.h>
#include <../Navigation/Navigation.h>
#include <Logging.h>

#include <string.h>
#include <cmath>

#define _USE_MATH_DEFINES

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
    sharedMem.teamColor = eBlueTeam;

    sharedMem.systemState = eDribbleBall;

    TEntry *entry = &sharedMem.positioning[sharedMem.currentIdx];

    m_visionComm->ReadData(&entry->visionData);
    m_logWdgt->ShowMsg(QString("Received data: yellow: x = %1 y = %2 orientation = %3\n"
                               "blue: x = %4 y = %5 orientation = %6 \n"
                               "ball: x = %7 y= %8\n"
                               "timestamp %9\n\n")
                       .arg(entry->visionData.yellow_x)
                       .arg(entry->visionData.yellow_y)
                       .arg(entry->visionData.yellow_angle)
                       .arg(entry->visionData.blue_x)
                       .arg(entry->visionData.blue_y)
                       .arg(entry->visionData.blue_angle)
                       .arg(entry->visionData.ball_x)
                       .arg(entry->visionData.ball_y)
                       .arg(entry->visionData.timestamp));
}

void MainWindow::NavToBallSlot()
{


    m_timer.start(TIMER_INTERVAL_MS);
}

void MainWindow::StopeMvmntSlot()
{
    m_timer.stop();
    sharedMem.systemState = eStop;

    TEntry *entry = &sharedMem.positioning[sharedMem.currentIdx];
    entry->robotData.motor_fl = 0;
    entry->robotData.motor_fr = 0;
    m_btComm->SendData(&entry->robotData);
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
    sharedMem.systemState = eNavToBall;
    TEntry *entry = &sharedMem.positioning[sharedMem.currentIdx];

    m_visionComm->ReadData(&entry->visionData);
    if(entry->visionData.yellow_angle > M_PI)
        entry->visionData.yellow_angle =   (-M_PI*2 +entry->visionData.yellow_angle);
    if(entry->visionData.blue_angle > M_PI)
        entry->visionData.blue_angle =   (-M_PI*2 +entry->visionData.blue_angle);

    //if(entry->visionData.blue_angle < 0)
    //    entry->visionData.blue_angle =   (M_PI*2 +entry->visionData.blue_angle);
    //if(entry->visionData.yellow_angle < 0)
    //    entry->visionData.yellow_angle =   (M_PI*2 +entry->visionData.yellow_angle);

    m_logWdgt->ShowMsg(QString("Received data: yellow: x = %1 y = %2 orientation = %3\n"
                               "blue: x = %4 y = %5 orientation = %6 \n"
                               "ball: x = %7 y= %8\n"
                               "newOrientation %9 %10 %11\n\n")
                       .arg(entry->visionData.yellow_x)
                       .arg(entry->visionData.yellow_y)
                       .arg(entry->visionData.yellow_angle)
                       .arg(entry->visionData.blue_x)
                       .arg(entry->visionData.blue_y)
                       .arg(entry->visionData.blue_angle)
                       .arg(entry->visionData.ball_x)
                       .arg(entry->visionData.ball_y)
                       .arg(entry->aiData.path[0].orientation)
                        .arg(entry->aiData.path[1].orientation)
            .arg(entry->aiData.path[2].orientation)
            );
    entry->aiData.path[0].position_X = entry->visionData.ball_x;
    entry->aiData.path[0].position_Y = entry->visionData.ball_y;



    //Vector2 normalisedLocation = ourRobotLocation - targetLocation;
    //float newAngle = atan2(normalisedLocation.Y(), normalisedLocation.X())));

    m_nav.GenerateValues();

    m_btComm->SendData(&entry->robotData);
    /*
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
    */
}
