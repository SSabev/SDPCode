#include "PitchSideDlg.h"

#include "../../Shared/SharedMem.h"

CPitchSideDlg::CPitchSideDlg(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    if(sharedMem.pitchSide == eLeftSide)
        leftBtn->setChecked(true);
    else
        rightBtn->setChecked(true);

    connect(okBtn, SIGNAL(clicked()), this, SLOT(OkSlot()));
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(CancelSlot()));
}

void CPitchSideDlg::OkSlot()
{
    if(leftBtn->isChecked())
        sharedMem.pitchSide = eLeftSide;
    else
        sharedMem.pitchSide = eRightSide;
    accept();
}

void CPitchSideDlg::CancelSlot()
{
    reject();
}
