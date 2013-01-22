#ifndef LOGGING_H
#define LOGGING_H

#include <QWidget>

#include "ui_Logging.h"

class CLogging
        : public QWidget
        , public Ui::Logging
{
    Q_OBJECT

public:
    CLogging(QWidget *parent = 0);

public slots:
    void AddMsg(const char *msg);
};

#endif // LOGGING_H
