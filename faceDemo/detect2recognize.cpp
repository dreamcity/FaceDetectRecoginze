#include "detect2recognize.h"
#include "ui_detect2recognize.h"


Detect2Recognize::Detect2Recognize(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Detect2Recognize)
{
    // the default construct function
    ui->setupUi(this);
}

Detect2Recognize::~Detect2Recognize()
{
    //the default deconstruct function
    delete ui;
}

void Detect2Recognize::on_dataPrepare_clicked()
{
    // close the currently dialog , current windows
    // then construct faceDetect dialog
    this->close();
    faceDetect.show();
    faceDetect.exec();
}

void Detect2Recognize::on_recognize_clicked()
{
    // close the currently dialog , current windows
    // then construct faceRecognize dialog
    this->close();
    faceRecognize.show();
    faceRecognize.exec();
}
