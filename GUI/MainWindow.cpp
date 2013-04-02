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

MainWindow::MainWindow()
{
    SetupGUI();
    InitSytem();
}

MainWindow::~MainWindow()
{

}

void MainWindow::MatchTestSlot()
{
    sharedMem.aiIdx = (sharedMem.aiIdx + 1)&SH_MEM_SIZE_MASK;
    TAIEntry *ai = &sharedMem.AIdata[sharedMem.aiIdx];

    m_pVisionComm->ReadData(&ai->visionData);

    sharedMem.systemState = eMatch;

    aiCtrl.RunAI(ai);

    m_nav.GenerateValues(&sharedMem.NavData[sharedMem.navIdx]);

    vision->UpdateWindow();

    sharedMem.systemState = eStop;
}

void MainWindow::PenaltyDefTestSlot()
{
    sharedMem.aiIdx = (sharedMem.aiIdx + 1)&SH_MEM_SIZE_MASK;
    TAIEntry *ai = &sharedMem.AIdata[sharedMem.aiIdx];

    m_pVisionComm->ReadData(&ai->visionData);
    sharedMem.systemState = ePenaltyDefend;
    aiCtrl.RunAI(ai);

    vision->UpdateWindow();
    sharedMem.systemState = eStop;
}

void MainWindow::PenaltyTestSlot()
{
    sharedMem.aiIdx = (sharedMem.aiIdx + 1)&SH_MEM_SIZE_MASK;
    TAIEntry *ai = &sharedMem.AIdata[sharedMem.aiIdx];

    m_pVisionComm->ReadData(&ai->visionData);
    sharedMem.systemState = ePenaltyAttack;
    aiCtrl.RunAI(ai);

    vision->UpdateWindow();
    sharedMem.systemState = eStop;
}

void MainWindow::SetupGUI()
{
    setupUi(this);

    connect(matchBtn, SIGNAL(clicked()), this, SLOT(StartMatchSlot()));
    connect(stopBtn, SIGNAL(clicked()), this, SLOT(StopMvmntSlot()));
    connect(doPenaltyBtn, SIGNAL(clicked()), this, SLOT(DoPenaltySlot()));
    connect(defendPenaltyBtn, SIGNAL(clicked()), this, SLOT(DefendPenaltySlot()));

    connect(connToVisionBtn, SIGNAL(clicked()), this, SLOT(ConnToVision()));
    connect(btConnectBtn, SIGNAL(clicked()), this, SLOT(ConnToBT()));
    connect(teamSetupBtn, SIGNAL(clicked()), this, SLOT(TeamSetup()));

    connect(matchTestBtn, SIGNAL(clicked()), this, SLOT(MatchTestSlot()));
    connect(defPenTestBtn, SIGNAL(clicked()), this, SLOT(PenaltyDefTestSlot()));
    connect(penTestBtn, SIGNAL(clicked()), this, SLOT(PenaltyTestSlot()));
}

void MainWindow::InitSytem()
{
    loggingObj = (ILogging *) LoggingWidget;

    // zero-out the shared memory
    memset(&sharedMem, 0, sizeof(TShMem));
    // set current state as IDLE
    sharedMem.systemState = eIDLE;
    sharedMem.speed_scale = 1.0;

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

void MainWindow::StartProcess(TSystemState state)
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

    // We are ready to start the process
    sharedMem.systemState = state;

    // disable all control buttons except STOP button
    CtrlButtons(false);

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
    CAICallback *AIcb = new CAICallback(m_pVisionComm, m_pIBtComm, &aiCtrl);
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

void MainWindow::StopMvmntSlot()
{
    if(sharedMem.systemState == eStop){
        sharedMem.navIdx = (sharedMem.navIdx + 1)&SH_MEM_SIZE_MASK;
        TNavEntry *nav = &sharedMem.NavData[sharedMem.navIdx];
        m_nav.GenerateStop(nav);
        m_pIBtComm->SendData(&nav->robot.sendData);
    }
    else{
        sharedMem.systemState = eStop;
        // Enable the control buttons
        CtrlButtons(true);
    }
}

void MainWindow::StartMatchSlot()
{
    StartProcess(eMatch);
}

void MainWindow::DefendPenaltySlot()
{
    StartProcess(ePenaltyDefend);
}

void MainWindow::DoPenaltySlot()
{
    StartProcess(ePenaltyAttack);
}

void MainWindow::UpdatePlotter()
{
    vision->UpdateWindow();
}

void MainWindow::CtrlButtons(bool enable)
{
    matchBtn->setEnabled(enable);
    defendPenaltyBtn->setEnabled(enable);
    doPenaltyBtn->setEnabled(enable);
    matchTestBtn->setEnabled(enable);
    defPenTestBtn->setEnabled(enable);
    penTestBtn->setEnabled(enable);
}
