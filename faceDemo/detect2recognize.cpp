#include "detect2recognize.h"
#include "ui_detect2recognize.h"


Detect2Recognize::Detect2Recognize(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Detect2Recognize)
{
    ui->setupUi(this);
}

Detect2Recognize::~Detect2Recognize()
{
    delete ui;
}

void Detect2Recognize::on_dataPrepare_clicked()
{

    this->close();
    faceDetect.show();
    faceDetect.exec();
}

void Detect2Recognize::on_recognize_clicked()
{
    this->close();
    faceRecognize.show();
    faceRecognize.exec();
}
