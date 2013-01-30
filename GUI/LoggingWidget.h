#ifndef LOGGINGWIDGET_H
#define LOGGINGWIDGET_H

#include <QWidget>

#include "ui_Logging.h"
#include "../Shared/Logging.h"

class CLoggingWidget
        : public QWidget
        , public Ui::Logging
        , ILogging
{
    Q_OBJECT

public:
    CLoggingWidget(QWidget *parent = 0);

    virtual void ShowMsg(const char *msg);
    void ShowMsg(QString msg);
};

#endif // LOGGING_H
