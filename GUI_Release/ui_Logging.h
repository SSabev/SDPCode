/********************************************************************************
** Form generated from reading UI file 'Logging.ui'
**
** Created: 
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGGING_H
#define UI_LOGGING_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Logging
{
public:
    QVBoxLayout *verticalLayout;
    QTextBrowser *msgBrowser;

    void setupUi(QWidget *Logging)
    {
        if (Logging->objectName().isEmpty())
            Logging->setObjectName(QString::fromUtf8("Logging"));
        Logging->resize(454, 348);
        verticalLayout = new QVBoxLayout(Logging);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        msgBrowser = new QTextBrowser(Logging);
        msgBrowser->setObjectName(QString::fromUtf8("msgBrowser"));

        verticalLayout->addWidget(msgBrowser);


        retranslateUi(Logging);

        QMetaObject::connectSlotsByName(Logging);
    } // setupUi

    void retranslateUi(QWidget *Logging)
    {
        Logging->setWindowTitle(QApplication::translate("Logging", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Logging: public Ui_Logging {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGGING_H
