#include "LoggingWdgt.h"

#include <QTextCharFormat>
#include <QMutexLocker>

CLoggingWdgt::CLoggingWdgt(QWidget *parent)
    : QTextBrowser(parent)
    , m_mutex(QMutex::Recursive)
{
}

void CLoggingWdgt::ShowMsg(const char *msg)
{
    QMutexLocker locker(&m_mutex);
    append(msg);
}

void CLoggingWdgt::ShowCriticalError(const char *err)
{
    QMutexLocker locker(&m_mutex);
    append(QString("<p><strong><font color=\"red\">%1</font></strong></p>")
           .arg(err));
}

void CLoggingWdgt::ShowMyMsg(QString msg)
{
    QMutexLocker locker(&m_mutex);
    append(msg);
}

void CLoggingWdgt::CriticalError(QString msg)
{
    QMutexLocker locker(&m_mutex);
    append(QString("<p><strong><font color=\"red\">%1</font></strong></p>").arg(msg));
}
