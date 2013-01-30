#ifndef PITCHSIDE_H
#define PITCHSIDE_H

#include <QDialog>
#include "ui_PitchSideDlg.h"

class CPitchSideDlg
        : public QDialog
        , public Ui::PitchSideDlg
{
    Q_OBJECT
public:
    CPitchSideDlg(QWidget *parent = 0);

private slots:
    void OkSlot();
    void CancelSlot();
};

#endif // PITCHSIDE_H
