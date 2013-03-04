#ifndef LOGGINGWDGT_H
#define LOGGINGWDGT_H

#include <QTextBrowser>

#include <Logging.h>

class CLoggingWdgt
        : public QTextBrowser
        , ILogging
{
    Q_OBJECT

public:
    CLoggingWdgt(QWidget *parent = 0);

    virtual void ShowMsg(const char *msg);

    void ShowMsg(QString msg);
};


#endif // LOGGINGWDGT_H
