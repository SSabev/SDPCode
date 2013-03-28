#ifndef LOGGINGWDGT_H
#define LOGGINGWDGT_H

#include <QTextBrowser>
#include <QMutex>

#include <Logging.h>

class CLoggingWdgt
        : public QTextBrowser
        , ILogging
{
    Q_OBJECT

public:
    CLoggingWdgt(QWidget *parent = 0);

    virtual void ShowMsg(const char *msg);
    virtual void ShowCriticalError(const char *err);

    void ShowMyMsg(QString msg);

    void CriticalError(QString msg);

    QMutex      m_mutex;
};


#endif // LOGGINGWDGT_H
