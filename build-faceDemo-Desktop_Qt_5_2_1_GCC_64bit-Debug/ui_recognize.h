/********************************************************************************
** Form generated from reading UI file 'recognize.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RECOGNIZE_H
#define UI_RECOGNIZE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Recognize
{
public:
    QLabel *label;
    QPushButton *quit;
    QPushButton *faceRecognizer;
    QLabel *label1;
    QPushButton *startCAM;
    QLabel *label2;
    QLabel *label3;
    QLabel *label4;
    QLabel *label5;
    QLabel *label6;
    QLabel *label7;
    QLabel *label8;

    void setupUi(QDialog *Recognize)
    {
        if (Recognize->objectName().isEmpty())
            Recognize->setObjectName(QStringLiteral("Recognize"));
        Recognize->resize(600, 350);
        label = new QLabel(Recognize);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 0, 300, 300));
        label->setAlignment(Qt::AlignCenter);
        quit = new QPushButton(Recognize);
        quit->setObjectName(QStringLiteral("quit"));
        quit->setGeometry(QRect(370, 320, 80, 25));
        faceRecognizer = new QPushButton(Recognize);
        faceRecognizer->setObjectName(QStringLiteral("faceRecognizer"));
        faceRecognizer->setGeometry(QRect(130, 320, 120, 25));
        label1 = new QLabel(Recognize);
        label1->setObjectName(QStringLiteral("label1"));
        label1->setGeometry(QRect(320, 0, 120, 120));
        startCAM = new QPushButton(Recognize);
        startCAM->setObjectName(QStringLiteral("startCAM"));
        startCAM->setGeometry(QRect(30, 320, 81, 23));
        label2 = new QLabel(Recognize);
        label2->setObjectName(QStringLiteral("label2"));
        label2->setGeometry(QRect(320, 120, 120, 25));
        label3 = new QLabel(Recognize);
        label3->setObjectName(QStringLiteral("label3"));
        label3->setGeometry(QRect(460, 0, 120, 120));
        label4 = new QLabel(Recognize);
        label4->setObjectName(QStringLiteral("label4"));
        label4->setGeometry(QRect(460, 120, 120, 25));
        label5 = new QLabel(Recognize);
        label5->setObjectName(QStringLiteral("label5"));
        label5->setGeometry(QRect(320, 150, 120, 120));
        label6 = new QLabel(Recognize);
        label6->setObjectName(QStringLiteral("label6"));
        label6->setGeometry(QRect(320, 270, 120, 25));
        label7 = new QLabel(Recognize);
        label7->setObjectName(QStringLiteral("label7"));
        label7->setGeometry(QRect(460, 150, 120, 120));
        label8 = new QLabel(Recognize);
        label8->setObjectName(QStringLiteral("label8"));
        label8->setGeometry(QRect(460, 270, 120, 25));

        retranslateUi(Recognize);
        QObject::connect(quit, SIGNAL(clicked()), Recognize, SLOT(close()));

        QMetaObject::connectSlotsByName(Recognize);
    } // setupUi

    void retranslateUi(QDialog *Recognize)
    {
        Recognize->setWindowTitle(QApplication::translate("Recognize", "Dialog", 0));
        label->setText(QString());
        quit->setText(QApplication::translate("Recognize", "quit", 0));
        faceRecognizer->setText(QApplication::translate("Recognize", "FaceRecognizer", 0));
        label1->setText(QString());
        startCAM->setText(QApplication::translate("Recognize", "StartCAM", 0));
        label2->setText(QApplication::translate("Recognize", "Person:", 0));
        label3->setText(QString());
        label4->setText(QApplication::translate("Recognize", "Person:", 0));
        label5->setText(QString());
        label6->setText(QApplication::translate("Recognize", "Person:", 0));
        label7->setText(QString());
        label8->setText(QApplication::translate("Recognize", "Person:", 0));
    } // retranslateUi

};

namespace Ui {
    class Recognize: public Ui_Recognize {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RECOGNIZE_H
