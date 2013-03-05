#include "LoggingWdgt.h"

#include <QTextCharFormat>

CLoggingWdgt::CLoggingWdgt(QWidget *parent)
    : QTextBrowser(parent)
{
}

void CLoggingWdgt::ShowMsg(const char *msg)
{
    append(msg);
}

void CLoggingWdgt::ShowMyMsg(QString msg)
{
    append(msg);
}

void CLoggingWdgt::ShowCriticalError(QString msg)
{
    append(QString("<p><strong><font color=\"red\">%1</font></strong></p>").arg(msg));
}
