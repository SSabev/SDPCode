#include "LoggingWidget.h"

CLoggingWidget::CLoggingWidget(QWidget *parent)
    : QWidget(parent, Qt::Window)
{
    setupUi(this);

    setWindowTitle("Log Messages");
}

void CLoggingWidget::ShowMsg(const char *msg)
{
    msgBrowser->append(msg);
}

void CLoggingWidget::ShowMsg(QString msg)
{
    msgBrowser->append(msg);
}
