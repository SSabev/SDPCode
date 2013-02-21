#include <QApplication>
#include <QMainWindow>

#include "MainWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMainWindow win;
    CMainWidget widget(&win);
    win.setCentralWidget((QWidget*)&widget);
    win.show();
//    widget.show();
    return app.exec();
}
