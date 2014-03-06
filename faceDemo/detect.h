#ifndef DETECT_H
#define DETECT_H
#define MYSQLPP_MYSQL_HEADERS_BURIED
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>

#include <mysql++/mysql++.h>
#include <QButtonGroup>
#include <qbuttongroup.h>
#include <QDialog>

using namespace std;
using namespace cv;
namespace Ui {
class Detect;
}

class Detect : public QDialog
{
    Q_OBJECT

public:
    explicit Detect(QWidget *parent = 0);
    void detectFace(Mat frame, vector<Rect> &faces);
    void createImgDataFile(const char* imgfilepath, const char* imgdatafile);
    void savedatabase();
    void listDir(const char* path);
    void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels);
    string faceTrain(std::vector<Mat> images,std::vector<int> labels);
    void faceReg(const string& configfile,std::vector<Mat> showimages, std::vector<Mat> testimages);
    ~Detect();

private slots:

    void on_recognize_clicked();

    void on_startCam_clicked();

    void on_changePerson_clicked();

    void on_takePic_clicked();

    void on_savePic_clicked();



    void on_trainData_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Detect *ui;
//    Recognize faceRecognize;
    VideoCapture cap;
    Mat frame;
    int indexLabel;
    int indexPerson;
    bool ON_OFF;
    String face_cascade_name;
    CascadeClassifier face_cascade;
    vector<Rect> faces;
    ofstream imgDataFile;
    string trainconfigfile;

};

#endif // DETECT_H
