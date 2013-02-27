#include "MainWindow.h"
#include "Tools/TeamCfgDlg.h"
#include <cmath>
#include <SharedMem.h>
#include <Logging.h>

#include <string.h>

#define TIMER_INTERVAL_MS 200

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

//take penlty kick?
    TEntry *entry = &sharedMem.positioning[sharedMem.currentIdx];    sharedMem.systemState = eStop;
	entry->robotData.motor_left =  0;
        entry->robotData.motor_right =  0;
        entry->robotData.motor_front =  0;
        entry->robotData.motor_rear =  0;
	entry->robotData.kicker = 2;
         m_btComm->SendData(&entry->robotData);

}

void MainWindow::NavToBallSlot()
{

    sharedMem.systemState = eNavToBall;
    TEntry *entry = &sharedMem.positioning[sharedMem.currentIdx];
    m_timer.start(TIMER_INTERVAL_MS);
    m_visionComm->ReadData(&entry->visionData);
    //m_visionComm->ReadData(&sharedMem.positioning[
    //                                sharedMem.currentIdx]
    //                          .visionData);
    if(entry->visionData.yellow_angle <0)
         entry->aiData.path[0].orientation = 2*M_PI + entry->visionData.yellow_angle;
    else
        entry->aiData.path[0].orientation = entry->visionData.yellow_angle;
    entry->aiData.path[0].position_X = entry->visionData.yellow_x;
    entry->aiData.path[0].position_Y = entry->visionData.yellow_y;

    entry->aiData.path[1].position_X = entry->visionData.ball_x;
    entry->aiData.path[1].position_Y = entry->visionData.ball_y;
    loggingObj->ShowMsg(QString("yellow x %1,yellow y %2,orient yellow %3,blue x %4,blue y %5,blue orient %6")
                            .arg(entry->visionData.yellow_x)
                            .arg(entry->visionData.yellow_y)
                .arg(entry->visionData.yellow_angle)
                .arg(entry->visionData.blue_x)
                .arg(entry->visionData.blue_y)
                 .arg(entry->visionData.blue_angle)
            .toAscii()
                            .data());
    entry->aiData.path[0].position_X = 0;
    entry->aiData.path[0].position_Y = 0;
    entry->aiData.path[1].position_X =5;
    entry->aiData.path[1].position_Y = 5;
     entry->aiData.path[0].orientation = -0.1;
    while(entry->aiData.path[0].orientation<2*M_PI-0.1)
    {
    entry->aiData.path[0].orientation +=0.1;
    nav.Holonomic();
    loggingObj->ShowMsg(QString("x %1,y %2,orient %3,x_target %4,y_target %5,orient_target %6, speed1 %7, speed2 %8,speed3 %9  ,speed4 %10 ")
                            .arg(entry->aiData.path[0].position_X)
                            .arg(entry->aiData.path[0].position_Y)
                .arg(entry->aiData.path[0].orientation)
                .arg(entry->aiData.path[1].position_X)
                .arg(entry->aiData.path[1].position_Y)
                .arg(entry->aiData.path[1].orientation)
                 .arg(entry->robotData.motor_left)
             .arg(entry->robotData.motor_right)
             .arg(entry->robotData.motor_front)
             .arg(entry->robotData.motor_rear)
            .toAscii()
                            .data());
    }
    //aiCtrl.RunAI();


    loggingObj->ShowMsg(QString("x %1,y %2,orient %3,x_target %4,y_target %5,orient_target %6, speed1 %7, speed2 %8,speed3 %9  ,speed4 %10 ")
                            .arg(entry->aiData.path[0].position_X)
                            .arg(entry->aiData.path[0].position_Y)
			    .arg(entry->aiData.path[0].orientation)
                .arg(entry->aiData.path[1].position_X)
                .arg(entry->aiData.path[1].position_Y)
                .arg(entry->aiData.path[1].orientation)
                 .arg(entry->robotData.motor_left)
             .arg(entry->robotData.motor_right)
             .arg(entry->robotData.motor_front)
             .arg(entry->robotData.motor_rear)
            .toAscii()
                            .data());



    m_btComm->SendData(&entry->robotData);
    //m_timer.start();

}

void MainWindow::StopeMvmntSlot()
{

    TEntry *entry = &sharedMem.positioning[sharedMem.currentIdx];    sharedMem.systemState = eStop;
	entry->robotData.motor_left =  0;
        entry->robotData.motor_right =  0;
        entry->robotData.motor_front =  0;
        entry->robotData.motor_rear =  0;	
		entry->robotData.kicker = 0;
         m_btComm->SendData(&entry->robotData);
         m_timer.stop();
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

    m_btComm = new CBtComm(this);
    m_visionComm = new CVisionComm(this);

    sharedMem.currentIdx = 0;

    sharedMem.pitchCfg.pitchWidth = 600;
    sharedMem.pitchCfg.pitchHeight = 300;

    aiCtrl.Initialise();

    vision = new CVisionMod(this);
    vision->show();

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

void MainWindow::TimerCallBack()
{
    if(!sharedMem.systemStatus == eOperational){
        return;
    }
    TEntry *entry = &sharedMem.positioning[sharedMem.currentIdx];
    // Read Modules new information
    m_btComm->ReadData(
                &sharedMem.positioning[
                    sharedMem.currentIdx]
                .robotState);

    m_visionComm->ReadData(&entry->visionData);
    //aiCtrl.RunAI();
    if (sharedMem.teamColor == eYellowTeam)
    {
    if(entry->visionData.yellow_angle <0)
         entry->aiData.path[0].orientation = 2*M_PI + entry->visionData.yellow_angle;
    else
        entry->aiData.path[0].orientation = entry->visionData.yellow_angle;
    entry->aiData.path[0].position_X = entry->visionData.yellow_x;
    entry->aiData.path[0].position_Y = entry->visionData.yellow_y;
    }
    else
    {
        if(entry->visionData.blue_angle <0)
             entry->aiData.path[0].orientation = 2*M_PI + entry->visionData.blue_angle;
        else
            entry->aiData.path[0].orientation = entry->visionData.blue_angle;
       entry->aiData.path[0].position_X = entry->visionData.blue_x;
        entry->aiData.path[0].position_Y = entry->visionData.blue_y;
    }
    entry->aiData.path[1].position_X = entry->visionData.ball_x;
    entry->aiData.path[1].position_Y = entry->visionData.ball_y;
   //  nav.Holonomic();
    loggingObj->ShowMsg(QString("x %1,y %2,orient %3,x_target %4,y_target %5,orient_target %6, speed1 %7, speed2 %8,speed3 %9  ,speed4 %10 ")
                            .arg(entry->aiData.path[0].position_X)
                            .arg(entry->aiData.path[0].position_Y)
                .arg(entry->aiData.path[0].orientation)
                .arg(entry->aiData.path[1].position_X)
                .arg(entry->aiData.path[1].position_Y)
                .arg(entry->aiData.path[1].orientation)
                 .arg(entry->robotData.motor_left)
             .arg(entry->robotData.motor_right)
             .arg(entry->robotData.motor_front)
             .arg(entry->robotData.motor_rear)
            .toAscii()
                            .data());





    nav.Holonomic();
    m_btComm->SendData(&entry->robotData);

    // increment index
    sharedMem.currentIdx = (sharedMem.currentIdx+1) & SH_MEM_SIZE_MASK;
}
