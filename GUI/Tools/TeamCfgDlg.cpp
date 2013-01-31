#include "TeamCfgDlg.h"

#include "../../Shared/SharedMem.h"

CTeamCfgDlg::CTeamCfgDlg(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    if(sharedMem.pitchSide == eLeftSide)
        leftBtn->setChecked(true);
    else
        rightBtn->setChecked(true);

    if(sharedMem.teamColor == eYellowTeam)
        yellowBtn->setChecked(true);
    else
        blueBtn->setChecked(true);

    connect(okBtn, SIGNAL(clicked()), this, SLOT(OkSlot()));
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(CancelSlot()));
}

void CTeamCfgDlg::OkSlot()
{
    if(leftBtn->isChecked())
        sharedMem.pitchSide = eLeftSide;
    else
        sharedMem.pitchSide = eRightSide;

    if(yellowBtn->isChecked())
        sharedMem.teamColor = eYellowTeam;
    else
        sharedMem.teamColor = eBlueTeam;

    accept();
}

void CTeamCfgDlg::CancelSlot()
{
    reject();
}
