#include "MainWindow.h"
#include "../Shared/SharedMem.h"
#include "../Shared/Logging.h"

#include "Tools/PitchSideDlg.h"

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
    /*
    sharedMem.systemState = eMoveStraight;

    sharedMem.positioning[0].robotData.motor_1 = 1;
    m_btComm->SendData(&sharedMem.positioning[0].robotData);
    */
    if(!m_visionComm->ReadData(&sharedMem.pitchCfg))
        m_logWdgt->ShowMsg("Returned false");
    else
        m_logWdgt->ShowMsg(QString("Read: pitchWidth = %1\n pitchHeight = %2")
                           .arg(sharedMem.pitchCfg.pitchWidth)
                           .arg(sharedMem.pitchCfg.pitchHeight));

}

void MainWindow::PenaltySlot()
{
    sharedMem.systemState = eDoPenalty;

    sharedMem.positioning[0].robotData.motor_1 = 2;
    m_btComm->SendData(&sharedMem.positioning[0].robotData);
}

void MainWindow::StopeMvmntSlot()
{
    sharedMem.systemState = eStop;

    sharedMem.positioning[0].robotData.motor_1 = 3;
    m_btComm->SendData(&sharedMem.positioning[0].robotData);
}

void MainWindow::SetupGUI()
{
    setupUi(this);

    connect(moveStraightBtn, SIGNAL(clicked()), this, SLOT(MoveStraightSlot()));
    connect(penaltyBtn, SIGNAL(clicked()), this, SLOT(PenaltySlot()));
    connect(stopBtn, SIGNAL(clicked()), this, SLOT(StopeMvmntSlot()));

    m_logWdgt = new CLoggingWidget(this);
    m_logWdgt->show();

    connect(connToVisionBtn, SIGNAL(clicked()), this, SLOT(ConnToVision()));
    connect(btConnectBtn, SIGNAL(clicked()), this, SLOT(ConnToBT()));
    connect(pitchSideBtn, SIGNAL(clicked()), this, SLOT(SetPitchSide()));
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
    CPitchSideDlg dlg(this);
    dlg.exec();
}
