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

void MainWindow::Action1Slot()
{
    sharedMem.currentIdx = 0;
    TEntry *entry = &sharedMem.positioning[sharedMem.currentIdx];

    m_visionComm->ReadData(&entry->visionData);

    aiCtrl.RunAI();

    vision->UpdateWindow();
}

void MainWindow::Action2Slot()
{
    m_timer.start(TIMER_INTERVAL_MS);
}

void MainWindow::StopeMvmntSlot()
{
    TEntry *entry;

    m_timer.stop();

    sharedMem.systemState = eStop;

    entry = &sharedMem.positioning[sharedMem.currentIdx];

    // Generate stop values for the motors
    m_nav.GenerateStop();

    // Send motor values to robot
    mIBtComm->SendData(&entry->robot.sendData);

    // Increment index
    sharedMem.currentIdx = (sharedMem.currentIdx+1) & SH_MEM_SIZE_MASK;
}

void MainWindow::SetupGUI()
{
    setupUi(this);

    connect(actionBtn1, SIGNAL(clicked()), this, SLOT(Action1Slot()));
    connect(actionBtn2, SIGNAL(clicked()), this, SLOT(Action2Slot()));
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

    if(sharedMem.pitchCfg.pitchHeight == 0 ||
            sharedMem.pitchCfg.pitchWidth){
        /// THIS _MUST_ be fixed
        sharedMem.pitchCfg.pitchHeight = 326;
        sharedMem.pitchCfg.pitchWidth = 624;
    }
    vision->SetSize(sharedMem.pitchCfg.pitchWidth, sharedMem.pitchCfg.pitchHeight);
    vision->show();

    LoggingWidget->ShowMsg("Configured...");
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
    TEntry *entry;
    /// Frst check if we are operational

    if(mIBtComm->IsConnected())
        sharedMem.systemStatus = (TSystemStatus)((int)sharedMem.systemStatus | (int)eBTConnected);
    else{
        sharedMem.systemStatus = (TSystemStatus)((int)sharedMem.systemStatus & (int)eBTDisconnected);
        LoggingWidget->ShowCriticalError("NO CONNECTION TO ROBOT PRESENT");
    }

    if(m_visionComm->IsConnected())
        sharedMem.systemStatus = (TSystemStatus)((int)sharedMem.systemStatus | (int)eVisionPresent);
    else{
        sharedMem.systemStatus = (TSystemStatus)((int)sharedMem.systemStatus & (int)eVisionMissing);
        LoggingWidget->ShowCriticalError("NO CONNECTION TO VISION PRESENT");
    }

    if(sharedMem.systemStatus != eOperational){
        return;
    }

    /// Now start the workload

    entry = &sharedMem.positioning[sharedMem.currentIdx];

    // 1. Read new coordinates from vision
    m_visionComm->ReadData(&entry->visionData);

    // 2. Read robot state
    /// TODO: read data from robot

    // 3. Run AI to generate new set of points
    aiCtrl.RunAI();

    // 4. Generate motor values
    m_nav.GenerateValues();

    // 5. Send motor values to robot
    mIBtComm->SendData(&entry->robot.sendData);

    // 6. Increment index
    sharedMem.currentIdx = (sharedMem.currentIdx+1) & SH_MEM_SIZE_MASK;

    // Update visualization window with new points
    vision->UpdateWindow();
}
