#include "LoggingWdgt.h"

CLoggingWdgt::CLoggingWdgt(QWidget *parent)
    : QTextBrowser(parent)
{

}

void CLoggingWdgt::ShowMsg(const char *msg)
{
    append(msg);
}

void CLoggingWdgt::ShowMsg(QString msg)
{
    append(msg);
}
