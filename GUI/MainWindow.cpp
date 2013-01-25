#include "MainWindow.h"
#include "../Shared/SharedMem.h"
#include "../Shared/Logging.h"

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
    sharedMem.systemState = eMoveStraight;
}

void MainWindow::PenaltySlot()
{
    sharedMem.systemState = eDoPenalty;
}

void MainWindow::StopeMvmntSlot()
{
    sharedMem.systemState = eStop;
}

void MainWindow::SetupGUI()
{
    setupUi(this);

    connect(moveStraightBtn, SIGNAL(clicked()), this, SLOT(MoveStraightSlot()));
    connect(penaltyBtn, SIGNAL(clicked()), this, SLOT(PenaltySlot()));
    connect(stopBtn, SIGNAL(clicked()), this, SLOT(StopeMvmntSlot()));

    m_logWdgt = new CLoggingWidget(this);
    m_logWdgt->show();
}

void MainWindow::InitSytem()
{
    loggingObj = (ILogging *) m_logWdgt;

    // zero-out the shared memory
    memset(&sharedMem, 0, sizeof(TShMem));
    // set current state as IDLE
    sharedMem.systemState = eIDLE;

    m_btComm = new CBtComm();

}
