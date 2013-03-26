#include "MainWindow.h"
#include "Tools/TeamCfgDlg.h"
#include "ThreadedCallback.h"

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

#define TIMER_INTERVAL_MS 50

#define AI_CALL_PERIOD      50
#define NAV_CALL_PERIOD     10

MainWindow::MainWindow()
{
    SetupGUI();
    InitSytem();
}

MainWindow::~MainWindow()
{

}

void MainWindow::Action1Slot()
{
    sharedMem.aiIdx = (sharedMem.aiIdx + 1)&SH_MEM_SIZE_MASK;
    TAIEntry *ai = &sharedMem.AIdata[sharedMem.aiIdx];

    m_pVisionComm->ReadData(&ai->visionData);

    aiCtrl.RunAI(ai);

    vision->UpdateWindow();
}

void MainWindow::Action2Slot()
{
    StartGame();
}

void MainWindow::StopMvmntSlot()
{
    sharedMem.systemState = eStop;

    actionBtn1->setEnabled(true);
    actionBtn2->setEnabled(true);
    teamSetupBtn->setEnabled(true);
}

void MainWindow::SetupGUI()
{
    setupUi(this);

    connect(actionBtn1, SIGNAL(clicked()), this, SLOT(Action1Slot()));
    connect(actionBtn2, SIGNAL(clicked()), this, SLOT(Action2Slot()));
    connect(stopBtn, SIGNAL(clicked()), this, SLOT(StopMvmntSlot()));

    connect(connToVisionBtn, SIGNAL(clicked()), this, SLOT(ConnToVision()));
    connect(btConnectBtn, SIGNAL(clicked()), this, SLOT(ConnToBT()));
    connect(teamSetupBtn, SIGNAL(clicked()), this, SLOT(TeamSetup()));
}

void MainWindow::InitSytem()
{
    loggingObj = (ILogging *) LoggingWidget;

    // zero-out the shared memory
    memset(&sharedMem, 0, sizeof(TShMem));
    // set current state as IDLE
    sharedMem.systemState = eIDLE;

#ifdef ARDUINO_BLD
    CArduinoComm *btComm = new CArduinoComm(this);
    m_pIBtComm = (IBTComm *) btComm;
#endif
#ifdef NXT_BUILD
    CBtComm *btComm = new CBtComm(this);
    m_pIBtComm = (IBTComm *) btComm;
#endif

    m_pVisionComm = new CVisionComm(this);

    aiCtrl.Initialise();

    vision = new CVisionMod(this);

    if(sharedMem.pitchCfg.pitchHeight == 0 || sharedMem.pitchCfg.pitchWidth == 0){
        // Just in case
        sharedMem.pitchCfg.pitchHeight = 326;
        sharedMem.pitchCfg.pitchWidth  = 624;
    }
    vision->SetSize(sharedMem.pitchCfg.pitchWidth,
                    sharedMem.pitchCfg.pitchHeight);
    vision->show();

    LoggingWidget->ShowMsg("Configured...");
}

void MainWindow::ConnToBT()
{
    m_pIBtComm->ConnectToRobot();
}

void MainWindow::ConnToVision()
{
    m_pVisionComm->ConnectToVision();
}

void MainWindow::TeamSetup()
{
    CTeamCfgDlg dlg(this);
    dlg.exec();
}

void MainWindow::StartGame()
{    
    sharedMem.aiIdx = (sharedMem.aiIdx + 1)&SH_MEM_SIZE_MASK; // safe to do so because this is done once/before Nav could be called
    sharedMem.navIdx = (sharedMem.navIdx + 1)&SH_MEM_SIZE_MASK;

    TAIEntry *ai   = &sharedMem.AIdata[sharedMem.aiIdx];
    TNavEntry *nav = &sharedMem.NavData[sharedMem.navIdx];

    /// Frst check if we are operational

    if(m_pIBtComm->IsConnected())
        sharedMem.systemStatus = (TSystemStatus)((int)sharedMem.systemStatus | (int)eBTConnected);
    else{
        sharedMem.systemStatus = (TSystemStatus)((int)sharedMem.systemStatus & (int)eBTDisconnected);
        LoggingWidget->CriticalError("NO CONNECTION TO ROBOT PRESENT");
    }

    if(m_pVisionComm->IsConnected())
        sharedMem.systemStatus = (TSystemStatus)((int)sharedMem.systemStatus | (int)eVisionPresent);
    else{
        sharedMem.systemStatus = (TSystemStatus)((int)sharedMem.systemStatus & (int)eVisionMissing);
        LoggingWidget->CriticalError("NO CONNECTION TO VISION PRESENT");
    }

    if(sharedMem.systemStatus != eOperational){
        return;
    }

    // We are ready to start the game
    sharedMem.systemState = eMatch;
    // disable the buttons during the game
    actionBtn1->setDisabled(true);
    actionBtn2->setDisabled(true);
    teamSetupBtn->setDisabled(true);

    // 1. Read new coordinates from vision
    m_pVisionComm->ReadData(&ai->visionData);

    // 2. Read robot state
    /// TODO: read data from robot if needed
#ifndef DRY_RUN
    // 3. Run AI to generate new set of points
    aiCtrl.RunAI(ai);

    // 4. Immideatly generate motor values
    nav->visionData = ai->visionData;
    m_nav.GenerateValues(nav);
#endif
    // 5. Send motor values to robot
    m_pIBtComm->SendData(&nav->robot.sendData);

    // 6. Start Nav and AI threads
    StartThreads();

    // 7. Update plotter window with new data points
    vision->UpdateWindow();
}

void MainWindow::StartThreads()
{
    // AI thread
    m_pAIThread = new QThread;
    CAICallback *AIcb = new CAICallback(m_pVisionComm, &aiCtrl);
    AIcb->moveToThread(m_pAIThread);

    connect(AIcb, SIGNAL(finished()), m_pAIThread, SLOT(quit()));
    connect(AIcb, SIGNAL(finished()), AIcb, SLOT(deleteLater()));

    connect(AIcb, SIGNAL(UpdatePlotter()), this, SLOT(UpdatePlotter()));

    connect(m_pAIThread, SIGNAL(started()), AIcb, SLOT(process()));
    connect(m_pAIThread, SIGNAL(finished()), m_pAIThread, SLOT (deleteLater ()));
    connect(m_pAIThread, SIGNAL(terminated()), m_pAIThread, SLOT (deleteLater ()));

    m_pAIThread->start();

    // Nav thread

    m_pNavThread = new QThread;
    CNavCallback *Navcb = new CNavCallback(m_pVisionComm, m_pIBtComm, &m_nav);
    Navcb->moveToThread(m_pNavThread);

    connect(Navcb, SIGNAL(finished()), m_pNavThread, SLOT(quit()));
    connect(Navcb, SIGNAL(finished()), Navcb, SLOT(deleteLater()));

    connect(m_pNavThread, SIGNAL(started()), Navcb, SLOT(process()));
    connect(m_pNavThread, SIGNAL(finished()), m_pNavThread, SLOT (deleteLater ()));
    connect(m_pNavThread, SIGNAL(terminated()), m_pNavThread, SLOT (deleteLater ()));

    m_pNavThread->start();

}

void MainWindow::UpdatePlotter()
{
    vision->UpdateWindow();
}
