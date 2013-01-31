/********************************************************************************
** Form generated from reading UI file 'TeamCfgDlg.ui'
**
** Created: Thu Jan 31 13:26:10 2013
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEAMCFGDLG_H
#define UI_TEAMCFGDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_TeamCfgDlg
{
public:
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QRadioButton *leftBtn;
    QRadioButton *rightBtn;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *yellowBtn;
    QRadioButton *blueBtn;
    QFrame *line;
    QHBoxLayout *horizontalLayout;
    QPushButton *okBtn;
    QPushButton *cancelBtn;
    QSpacerItem *horizontalSpacer;

    void setupUi(QDialog *TeamCfgDlg)
    {
        if (TeamCfgDlg->objectName().isEmpty())
            TeamCfgDlg->setObjectName(QString::fromUtf8("TeamCfgDlg"));
        TeamCfgDlg->resize(242, 260);
        verticalLayout_3 = new QVBoxLayout(TeamCfgDlg);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        groupBox = new QGroupBox(TeamCfgDlg);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        leftBtn = new QRadioButton(groupBox);
        leftBtn->setObjectName(QString::fromUtf8("leftBtn"));

        verticalLayout->addWidget(leftBtn);

        rightBtn = new QRadioButton(groupBox);
        rightBtn->setObjectName(QString::fromUtf8("rightBtn"));

        verticalLayout->addWidget(rightBtn);


        verticalLayout_3->addWidget(groupBox);

        groupBox_2 = new QGroupBox(TeamCfgDlg);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        yellowBtn = new QRadioButton(groupBox_2);
        yellowBtn->setObjectName(QString::fromUtf8("yellowBtn"));

        verticalLayout_2->addWidget(yellowBtn);

        blueBtn = new QRadioButton(groupBox_2);
        blueBtn->setObjectName(QString::fromUtf8("blueBtn"));

        verticalLayout_2->addWidget(blueBtn);


        verticalLayout_3->addWidget(groupBox_2);

        line = new QFrame(TeamCfgDlg);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_3->addWidget(line);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        okBtn = new QPushButton(TeamCfgDlg);
        okBtn->setObjectName(QString::fromUtf8("okBtn"));

        horizontalLayout->addWidget(okBtn);

        cancelBtn = new QPushButton(TeamCfgDlg);
        cancelBtn->setObjectName(QString::fromUtf8("cancelBtn"));

        horizontalLayout->addWidget(cancelBtn);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_3->addLayout(horizontalLayout);


        retranslateUi(TeamCfgDlg);

        QMetaObject::connectSlotsByName(TeamCfgDlg);
    } // setupUi

    void retranslateUi(QDialog *TeamCfgDlg)
    {
        TeamCfgDlg->setWindowTitle(QApplication::translate("TeamCfgDlg", "Dialog", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("TeamCfgDlg", "Side On Pitch", 0, QApplication::UnicodeUTF8));
        leftBtn->setText(QApplication::translate("TeamCfgDlg", "Left Side", 0, QApplication::UnicodeUTF8));
        rightBtn->setText(QApplication::translate("TeamCfgDlg", "Right Side", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("TeamCfgDlg", "Team Color", 0, QApplication::UnicodeUTF8));
        yellowBtn->setText(QApplication::translate("TeamCfgDlg", "Yellow", 0, QApplication::UnicodeUTF8));
        blueBtn->setText(QApplication::translate("TeamCfgDlg", "Blue", 0, QApplication::UnicodeUTF8));
        okBtn->setText(QApplication::translate("TeamCfgDlg", "OK", 0, QApplication::UnicodeUTF8));
        cancelBtn->setText(QApplication::translate("TeamCfgDlg", "Cnacel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TeamCfgDlg: public Ui_TeamCfgDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEAMCFGDLG_H
