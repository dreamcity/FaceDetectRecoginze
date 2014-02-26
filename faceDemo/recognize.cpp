#include "recognize.h"
#include "ui_recognize.h"
#include "QMessageBox"
Recognize::Recognize(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Recognize)
{
    ui->setupUi(this);
    face_cascade_name = "haarcascade_frontalface_alt.xml";
}

Recognize::~Recognize()
{
    delete ui;
}



void Recognize::on_faceRecognizer_clicked()
{
    vector<Mat> images;
    Mat cropface;
    Mat cropface_gray;
    for(uint i=0; i<faces.size(); i++)
    {
        Mat crop(frame, faces[i]);
        cv::resize(crop, cropface, Size(120,120), 1, 1, CV_INTER_LINEAR);
        cvtColor( cropface, cropface_gray, CV_BGR2GRAY );
        equalizeHist(cropface_gray, cropface_gray );
        images.push_back(cropface_gray);
    }
    faceReg("../traindata.xml",images);
    QImage image = QImage((const uchar*)cropface.data, cropface.cols, cropface.rows, QImage::Format_RGB888).rgbSwapped();
    ui->label2->setPixmap(QPixmap::fromImage(image));

}

void Recognize::detectFace(Mat frame, vector<Rect>& faces)
{
    if( !face_cascade.load( face_cascade_name ) )
    {
//        cout<<"--(!)Error loading!"<<endl;
        QMessageBox::critical(NULL, "critical", "--(!)Error loading!",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        exit(0);
    }
    Mat frame_gray;
    cvtColor( frame, frame_gray, CV_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
    for( uint i = 0; i < faces.size(); i++ )
    {
        Point pt1(faces[i].x,faces[i].y);
        Point pt2(faces[i].x+faces[i].width,faces[i].y+faces[i].height);
        rectangle(frame,pt1,pt2,CV_RGB(0,255,0),3,2,0);
    }
}



void Recognize::faceReg(const string& configfile, std::vector<Mat> testimages)

{
    Ptr<FaceRecognizer> model = createEigenFaceRecognizer();
    model->load(configfile);

    int predictedLabel;
    if(testimages.size()>1)
    {
        QMessageBox::critical(NULL, "critical", "This version only support 1 person!",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    else
    {
        predictedLabel = model->predict(testimages[0]);
        string result_message = format("Predicted Person = %d  ", predictedLabel);
        QMessageBox::information(NULL, "information", result_message.c_str());
        ui->label3->setText(result_message.c_str());
    }
//    for (uint i = 0; i < testimages.size(); ++i)
//    {
//        predictedLabel = model->predict(testimages[i]);
//        //cout<<"The face"<<i<<" recognize is complete!"<<endl;
//        string result_message = format("Predicted Person = %d  ", predictedLabel);
//        QMessageBox::information(NULL, "information", result_message.c_str());
//        imshow(format("face%d", i), testimages[i]);

//    }
}

void Recognize::on_startCAM_clicked()
{
    cap.open(0);
    if(!cap.isOpened())
    {
//        cout<<"the camera cannot open"<<endl;
        QMessageBox::critical(NULL, "critical", "the camera cannot open",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    else
    {
         while(1)
         {
            cap >> frame; // get a new frame from camera
            detectFace(frame, faces);
            QImage image = QImage((const uchar*)frame.data, frame.cols, frame.rows, QImage::Format_RGB888).rgbSwapped();
            ui->label->setPixmap(QPixmap::fromImage(image));
            int c = waitKey(10);
            if( (char)c == 'c' )
                  break;
          }
    }
}
