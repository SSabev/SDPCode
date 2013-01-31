/********************************************************************************
** Form generated from reading UI file 'PitchSideDlg.ui'
**
** Created: Wed Jan 30 18:00:16 2013
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PITCHSIDEDLG_H
#define UI_PITCHSIDEDLG_H

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

class Ui_PitchSideDlg
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QRadioButton *leftBtn;
    QRadioButton *rightBtn;
    QFrame *line;
    QHBoxLayout *horizontalLayout;
    QPushButton *okBtn;
    QPushButton *cancelBtn;
    QSpacerItem *horizontalSpacer;

    void setupUi(QDialog *PitchSideDlg)
    {
        if (PitchSideDlg->objectName().isEmpty())
            PitchSideDlg->setObjectName(QString::fromUtf8("PitchSideDlg"));
        PitchSideDlg->resize(242, 160);
        verticalLayout_2 = new QVBoxLayout(PitchSideDlg);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox = new QGroupBox(PitchSideDlg);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        leftBtn = new QRadioButton(groupBox);
        leftBtn->setObjectName(QString::fromUtf8("leftBtn"));

        verticalLayout->addWidget(leftBtn);

        rightBtn = new QRadioButton(groupBox);
        rightBtn->setObjectName(QString::fromUtf8("rightBtn"));

        verticalLayout->addWidget(rightBtn);


        verticalLayout_2->addWidget(groupBox);

        line = new QFrame(PitchSideDlg);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        okBtn = new QPushButton(PitchSideDlg);
        okBtn->setObjectName(QString::fromUtf8("okBtn"));

        horizontalLayout->addWidget(okBtn);

        cancelBtn = new QPushButton(PitchSideDlg);
        cancelBtn->setObjectName(QString::fromUtf8("cancelBtn"));

        horizontalLayout->addWidget(cancelBtn);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout);


        retranslateUi(PitchSideDlg);

        QMetaObject::connectSlotsByName(PitchSideDlg);
    } // setupUi

    void retranslateUi(QDialog *PitchSideDlg)
    {
        PitchSideDlg->setWindowTitle(QApplication::translate("PitchSideDlg", "Dialog", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("PitchSideDlg", "Side On Pitch", 0, QApplication::UnicodeUTF8));
        leftBtn->setText(QApplication::translate("PitchSideDlg", "Left Side", 0, QApplication::UnicodeUTF8));
        rightBtn->setText(QApplication::translate("PitchSideDlg", "Right Side", 0, QApplication::UnicodeUTF8));
        okBtn->setText(QApplication::translate("PitchSideDlg", "OK", 0, QApplication::UnicodeUTF8));
        cancelBtn->setText(QApplication::translate("PitchSideDlg", "Cnacel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PitchSideDlg: public Ui_PitchSideDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PITCHSIDEDLG_H
