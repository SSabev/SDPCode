#include "Logging.h"

CLogging::CLogging(QWidget *parent)
    : QWidget(parent, Qt::Window)
{
    setupUi(this);

    setWindowTitle("Log Messages");
}

void CLogging::AddMsg(const char *msg)
{
    msgBrowser->append(msg);
}
