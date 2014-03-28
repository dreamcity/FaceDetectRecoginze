/********************************************************************************
** Form generated from reading UI file 'detect2recognize.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DETECT2RECOGNIZE_H
#define UI_DETECT2RECOGNIZE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Detect2Recognize
{
public:
    QWidget *centralWidget;
    QPushButton *dataPrepare;
    QPushButton *recognize;
    QPushButton *quit;
    QMenuBar *menuBar;
    QMenu *menuDetect2Recogize;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Detect2Recognize)
    {
        if (Detect2Recognize->objectName().isEmpty())
            Detect2Recognize->setObjectName(QStringLiteral("Detect2Recognize"));
        Detect2Recognize->resize(400, 300);
        centralWidget = new QWidget(Detect2Recognize);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        dataPrepare = new QPushButton(centralWidget);
        dataPrepare->setObjectName(QStringLiteral("dataPrepare"));
        dataPrepare->setGeometry(QRect(60, 70, 84, 23));
        recognize = new QPushButton(centralWidget);
        recognize->setObjectName(QStringLiteral("recognize"));
        recognize->setGeometry(QRect(210, 70, 81, 23));
        quit = new QPushButton(centralWidget);
        quit->setObjectName(QStringLiteral("quit"));
        quit->setGeometry(QRect(130, 130, 81, 23));
        Detect2Recognize->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Detect2Recognize);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 20));
        menuDetect2Recogize = new QMenu(menuBar);
        menuDetect2Recogize->setObjectName(QStringLiteral("menuDetect2Recogize"));
        Detect2Recognize->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Detect2Recognize);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Detect2Recognize->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Detect2Recognize);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Detect2Recognize->setStatusBar(statusBar);

        menuBar->addAction(menuDetect2Recogize->menuAction());

        retranslateUi(Detect2Recognize);
        QObject::connect(quit, SIGNAL(clicked()), Detect2Recognize, SLOT(close()));

        QMetaObject::connectSlotsByName(Detect2Recognize);
    } // setupUi

    void retranslateUi(QMainWindow *Detect2Recognize)
    {
        Detect2Recognize->setWindowTitle(QApplication::translate("Detect2Recognize", "Detect2Recognize", 0));
        dataPrepare->setText(QApplication::translate("Detect2Recognize", "DataPrepare", 0));
        recognize->setText(QApplication::translate("Detect2Recognize", "Recognize", 0));
        quit->setText(QApplication::translate("Detect2Recognize", "Quit", 0));
        menuDetect2Recogize->setTitle(QApplication::translate("Detect2Recognize", "Detect2Recognize", 0));
    } // retranslateUi

};

namespace Ui {
    class Detect2Recognize: public Ui_Detect2Recognize {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DETECT2RECOGNIZE_H
