#include "TeamCfgDlg.h"

#include <SharedMem.h>

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

    widthBx->setValue(sharedMem.pitchCfg.pitchWidth);
    heightBx->setValue(sharedMem.pitchCfg.pitchHeight);
    speedScaleSB->setValue(sharedMem.speed_scale);

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

    sharedMem.pitchCfg.pitchWidth = widthBx->value();
    sharedMem.pitchCfg.pitchHeight = heightBx->value();
    sharedMem.speed_scale = speedScaleSB->value();

    accept();
}

void CTeamCfgDlg::CancelSlot()
{
    reject();
}
