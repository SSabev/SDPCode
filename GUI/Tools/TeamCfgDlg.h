#ifndef PITCHSIDE_H
#define PITCHSIDE_H

#include <QDialog>
#include "ui_TeamCfgDlg.h"

class CTeamCfgDlg
        : public QDialog
        , public Ui::TeamCfgDlg
{
    Q_OBJECT
public:
    CTeamCfgDlg(QWidget *parent = 0);

private slots:
    void OkSlot();
    void CancelSlot();
};

#endif // PITCHSIDE_H
