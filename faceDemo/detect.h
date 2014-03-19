#ifndef DETECT_H
#define DETECT_H
// declare the compile g++ with the parameters about libs and routes
#define MYSQLPP_MYSQL_HEADERS_BURIED

#include "2dpcaface.h"
// some include .h files to process the file-system
//   --create file-folder
//   ----save the .jpg files in the file-folder
//   --search the file-folder and files
//   ----create the .xml file, perform as the .jpg facedata
//   ----read .xml file
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

// include mysql++ ,support the c++ interface
#include <mysql++/mysql++.h>
#include <QButtonGroup>
#include "QMessageBox"
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
    // detect the face region, return a series reatanges
    void detectFace(Mat frame, vector<Rect> &faces);
    // createImgDataFile , save the imgdatafile in the imgfilepath
    void createImgDataFile(const char* imgfilepath, const char* imgdatafile);
    // save the peoples infomation in the mysql dadabase;
    void savedatabase();
    //search the files in the file-folder
    void listDir(const char* path);
    // read_csv files, get the images and labels;
    // --images  the store path "../../**.jpg"
    // --labels int;
    void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels);
    // train face data, return a trainconfigfile,
    // store --mean, eigenvalues, eigenvector, num_component, project_matrix;
    string faceTrain(std::vector<Mat> images,std::vector<int> labels);
    // --recognize the facedata;
    void faceReg(const string& configfile,std::vector<Mat> showimages, std::vector<Mat> testimages);
    ~Detect();

private slots:

    void on_recognize_clicked();

    void on_startCam_clicked();

    void on_changePerson_clicked();

    void on_takePic_clicked();

    void on_savePic_clicked();

    void on_trainData_clicked();


private:
    Ui::Detect *ui;
    // declare the VideoCapture
    VideoCapture cap;
    Mat frame;
    // the index of label, range 0 to 8
    int indexLabel;
    // the index of person
    int indexPerson;
    bool ON_OFF;
    // the classifier , detect region of face
    String face_cascade_name;
    CascadeClassifier face_cascade;
    // the  rectangle region of face, detect by the classifier
    vector<Rect> faces;
    // the face database .xml file
    // sames as KEY<----->VALUE
    //          img-------label
    ofstream imgDataFile;
    //the traindata file ,store --mean, eigenvalues, eigenvector, num_component, project_matrix
    string trainconfigfile;

};

#endif // DETECT_H
