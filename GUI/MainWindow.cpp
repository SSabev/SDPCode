#include "MainWindow.h"

MainWindow::MainWindow()
{
    SetupGUI();
}

MainWindow::~MainWindow()
{

}

void MainWindow::MoveStraightSlot()
{

}

void MainWindow::PenaltySlot()
{

}

void MainWindow::StopeMvmntSlot()
{

}

void MainWindow::SetupGUI()
{
    setupUi(this);

    connect(moveStraightBtn, SIGNAL(clicked()), this, SLOT(MoveStraightSlot()));
    connect(penaltyBtn, SIGNAL(clicked()), this, SLOT(PenaltySlot()));
    connect(stopBtn, SIGNAL(clicked()), this, SLOT(StopeMvmntSlot()));

    m_logWdgt = new CLogging(this);
    m_logWdgt->show();
}
