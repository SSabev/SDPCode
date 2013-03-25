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
    : m_AI_Timer(this)
    , m_Nav_Timer(this)
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
    actionBtn1->setDisabled(true);
    m_AI_Timer.start(TIMER_INTERVAL_MS);
}

void MainWindow::StopMvmntSlot()
{
    unsigned nextIdx = (sharedMem.navIdx + 1)&SH_MEM_SIZE_MASK;
    TNavEntry *nav = &sharedMem.NavData[nextIdx];

    m_AI_Timer.stop();
    m_Nav_Timer.stop();

    sharedMem.systemState = eStop;

    // Generate stop values for the motors
    m_nav.GenerateStop(nav);

    // Send motor values to robot
    m_pIBtComm->SendData(&nav->robot.sendData);

    // Increment index
    sharedMem.navIdx = nextIdx;

    actionBtn1->setEnabled(true);
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

    m_AI_Timer.setSingleShot(false);
    connect(&m_AI_Timer, SIGNAL(timeout()), this, SLOT(AIStage1Callback()));

    m_Nav_Timer.setSingleShot(false);
    connect(&m_Nav_Timer, SIGNAL(timeout()), this, SLOT(NavTimerCallback()));
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

void MainWindow::AIStage1Callback()
{
    // no need any threading - fist time we must evaluate the path before calling navigation

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

    // 1. Read new coordinates from vision
    m_pVisionComm->ReadData(&ai->visionData);

    // 2. Read robot state
    /// TODO: read data from robot if needed

    // 3. Run AI to generate new set of points
    aiCtrl.RunAI(ai);

    // 4. Immideatly generate motor values
    nav->visionData = ai->visionData;
    m_nav.GenerateValues(nav);

    // 5. Send motor values to robot
    m_pIBtComm->SendData(&nav->robot.sendData);

    // 6. Start Nav timer
    m_Nav_Timer.start(NAV_CALL_PERIOD);

    // 7. Reset the AI callback to stage 2 (threaded version)
    disconnect(&m_AI_Timer, SIGNAL(timeout()), this, SLOT(AIStage1Callback()));
    connect   (&m_AI_Timer, SIGNAL(timeout()), this, SLOT(AIStage2Callback()));

    // 8. Update plotter window with new data points
    vision->UpdateWindow();
}

void MainWindow::AIStage2Callback()
{
    // Threaded state

    m_pAIThread = new QThread;
    CAICallback *cb = new CAICallback(m_pVisionComm, &aiCtrl, vision);
    cb->moveToThread(m_pAIThread);

    connect(cb, SIGNAL(finished()), m_pAIThread, SLOT(quit()));
    connect(cb, SIGNAL(finished()), cb, SLOT(deleteLater()));

    connect(m_pAIThread, SIGNAL(started()), cb, SLOT(process()));
    connect(m_pAIThread, SIGNAL(finished()), m_pAIThread, SLOT (deleteLater ()));
    connect(m_pAIThread, SIGNAL(terminated()), m_pAIThread, SLOT (deleteLater ()));

    m_pAIThread->start();
}

void MainWindow::NavTimerCallback()
{
    m_pNavThread = new QThread;
    CNavCallback *cb = new CNavCallback(m_pVisionComm, m_pIBtComm, &m_nav);
    cb->moveToThread(m_pNavThread);

    connect(cb, SIGNAL(finished()), m_pNavThread, SLOT(quit()));
    connect(cb, SIGNAL(finished()), cb, SLOT(deleteLater()));

    connect(m_pNavThread, SIGNAL(started()), cb, SLOT(process()));
    connect(m_pNavThread, SIGNAL(finished()), m_pNavThread, SLOT (deleteLater ()));
    connect(m_pNavThread, SIGNAL(terminated()), m_pNavThread, SLOT (deleteLater ()));

    m_pNavThread->start();
}
/*
void MainWindow::TimerCallBack()
{
    TEntry *entry;


    /// Now start the workload

    entry = &sharedMem.positioning[sharedMem.currentIdx];

    // 1. Read new coordinates from vision
    m_pVisionComm->ReadData(&entry->visionData);

    // 2. Read robot state
    /// TODO: read data from robot

    // 3. Run AI to generate new set of points
    aiCtrl.RunAI();

    // 4. Generate motor values
    m_nav.GenerateValues();

    // 5. Send motor values to robot
    m_pIBtComm->SendData(&entry->robot.sendData);

    // Update visualization window with new points
    vision->UpdateWindow();

    // 6. Increment index
    sharedMem.currentIdx = (sharedMem.currentIdx+1) & SH_MEM_SIZE_MASK;
}
*/
