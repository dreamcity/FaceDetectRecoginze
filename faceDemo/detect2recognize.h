#ifndef DETECT2RECOGNIZE_H
#define DETECT2RECOGNIZE_H
#include "detect.h"
#include "recognize.h"
#include <QMainWindow>

namespace Ui {
class Detect2Recognize;
}

class Detect2Recognize : public QMainWindow
{
    Q_OBJECT

public:
    explicit Detect2Recognize(QWidget *parent = 0);
    ~Detect2Recognize();

private slots:
    void on_dataPrepare_clicked();

    void on_recognize_clicked();

private:
    Ui::Detect2Recognize *ui;
    Detect faceDetect;
    Recognize faceRecognize;
};

#endif // DETECT2RECOGNIZE_H
