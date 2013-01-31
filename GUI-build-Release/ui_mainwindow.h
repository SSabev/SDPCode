/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Thu Jan 31 13:12:37 2013
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QPushButton *moveStraightBtn;
    QPushButton *penaltyBtn;
    QPushButton *stopBtn;
    QSpacerItem *verticalSpacer;
    QFrame *line;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout;
    QPushButton *btConnectBtn;
    QPushButton *connToVisionBtn;
    QPushButton *teamSetupBtn;
    QPushButton *logBtn;
    QSpacerItem *verticalSpacer_2;
    QMenuBar *menubar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(317, 271);
        QIcon icon;
        icon.addFile(QString::fromUtf8("img/robot_icon.gif"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        moveStraightBtn = new QPushButton(groupBox);
        moveStraightBtn->setObjectName(QString::fromUtf8("moveStraightBtn"));

        verticalLayout_2->addWidget(moveStraightBtn);

        penaltyBtn = new QPushButton(groupBox);
        penaltyBtn->setObjectName(QString::fromUtf8("penaltyBtn"));

        verticalLayout_2->addWidget(penaltyBtn);

        stopBtn = new QPushButton(groupBox);
        stopBtn->setObjectName(QString::fromUtf8("stopBtn"));

        verticalLayout_2->addWidget(stopBtn);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        horizontalLayout->addWidget(groupBox);

        line = new QFrame(centralwidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout = new QVBoxLayout(groupBox_2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        btConnectBtn = new QPushButton(groupBox_2);
        btConnectBtn->setObjectName(QString::fromUtf8("btConnectBtn"));

        verticalLayout->addWidget(btConnectBtn);

        connToVisionBtn = new QPushButton(groupBox_2);
        connToVisionBtn->setObjectName(QString::fromUtf8("connToVisionBtn"));

        verticalLayout->addWidget(connToVisionBtn);

        teamSetupBtn = new QPushButton(groupBox_2);
        teamSetupBtn->setObjectName(QString::fromUtf8("teamSetupBtn"));

        verticalLayout->addWidget(teamSetupBtn);

        logBtn = new QPushButton(groupBox_2);
        logBtn->setObjectName(QString::fromUtf8("logBtn"));

        verticalLayout->addWidget(logBtn);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        horizontalLayout->addWidget(groupBox_2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 317, 29));
        MainWindow->setMenuBar(menubar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Robotics", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "Robot Controls", 0, QApplication::UnicodeUTF8));
        moveStraightBtn->setText(QApplication::translate("MainWindow", "Read Data", 0, QApplication::UnicodeUTF8));
        penaltyBtn->setText(QApplication::translate("MainWindow", "Penalty", 0, QApplication::UnicodeUTF8));
        stopBtn->setText(QApplication::translate("MainWindow", "Stop", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Configuration", 0, QApplication::UnicodeUTF8));
        btConnectBtn->setText(QApplication::translate("MainWindow", "Connect to BT", 0, QApplication::UnicodeUTF8));
        connToVisionBtn->setText(QApplication::translate("MainWindow", "Connect to Vision", 0, QApplication::UnicodeUTF8));
        teamSetupBtn->setText(QApplication::translate("MainWindow", "Team Setup", 0, QApplication::UnicodeUTF8));
        logBtn->setText(QApplication::translate("MainWindow", "Show Logging", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
