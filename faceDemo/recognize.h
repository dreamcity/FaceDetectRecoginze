#ifndef RECOGNIZE_H
#define RECOGNIZE_H

#include <QDialog>
#include <detect.h>


using namespace std;
using namespace cv;

namespace Ui {
class Recognize;
}

class Recognize : public QDialog
{
    Q_OBJECT

public:
    explicit Recognize(QWidget *parent = 0);
    void detectFace(Mat frame, vector<Rect> &faces);
    void faceReg(const string& configfile, std::vector<Mat> showimages, std::vector<Mat> testimages);
    ~Recognize();

private slots:


    void on_faceRecognizer_clicked();

    void on_startCAM_clicked();

private:
    Ui::Recognize *ui;

    VideoCapture cap;
    Mat frame;
    String face_cascade_name;
    CascadeClassifier face_cascade;
    vector<Rect> faces;
    int indexperson;


};

#endif // RECOGNIZE_H
