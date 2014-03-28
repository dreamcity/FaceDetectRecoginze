#include "recognize.h"
#include "ui_recognize.h"
#include "QMessageBox"
Recognize::Recognize(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Recognize)
{
    ui->setupUi(this);
    // load the classifier , support by the offical opencv
    face_cascade_name = "haarcascade_frontalface_alt.xml";
    indexperson=0;
}

Recognize::~Recognize()
{
    delete ui;
}


// see void Detect::on_recognize_clicked()
// first load the traindata file, "traindata.xml"
// call faceReg to relize the really function
void Recognize::on_faceRecognizer_clicked()
{
    indexperson=0;
    const char* traindatafile="../traindata.xml";
    if((access(traindatafile,F_OK))==-1)
    {
        QMessageBox::critical(NULL, "critical", "NO Train data exit! Please train first!",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    else
    {
        QLabel *labelpic[4]  = {ui->label1, ui->label3,
                                ui->label5, ui->label7};
        QLabel *labelname[4] = {ui->label2, ui->label4,
                                ui->label6, ui->label8};
        //  string str0("");
        Mat img;
        string str1("Person:");
        for(int i=0; i<4; i++)
        {
            QImage image = QImage((const uchar*)img.data, img.cols, img.rows, QImage::Format_RGB888).rgbSwapped();
            labelpic[i]->setPixmap(QPixmap::fromImage(image));
            // labelpic[i]->setPixmap();
            // setText(str0.c_str());
            labelname[i]->setText(str1.c_str());
        }


        vector<Mat> images_rgb;
        vector<Mat> images_gray;
        Mat cropface_rgb;
        Mat cropface_gray;
        for(uint i=0; i<faces.size(); i++)
        {
            Mat crop(frame, faces[i]);
            cv::resize(crop, cropface_rgb, Size(120,120), 1, 1, CV_INTER_LINEAR);
            cvtColor( cropface_rgb, cropface_gray, CV_BGR2GRAY );
            equalizeHist(cropface_gray, cropface_gray );
            images_rgb.push_back(cropface_rgb);
            images_gray.push_back(cropface_gray);

            faceReg(traindatafile, images_rgb, images_gray);

        }

    }

}

// void Detect::detectFace(Mat frame, vector<Rect>& faces)
void Recognize::detectFace(Mat frame, vector<Rect>& faces)
{
    if( !face_cascade.load( face_cascade_name ) )
    {
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


// void Detect::faceReg(const string& configfile,std::vector<Mat> showimages, std::vector<Mat> testimages)
void Recognize::faceReg(const string& configfile,std::vector<Mat> showimages, std::vector<Mat> testimages)
{
    Ptr<FaceRecognizer> model = createPCA2DFaceRecognizer(0);
    model->load(configfile);

//     Ptr<FaceRecognizer> model = createEigenFaceRecognizer();
//     model->load(configfile);

    int predictedLabel = -1;
    QLabel *labelpic[4]  = {ui->label1, ui->label3,
                            ui->label5, ui->label7};
    QLabel *labelname[4] = {ui->label2, ui->label4,
                            ui->label6, ui->label8};
    Mat imagetmp_rgb;
    for (uint i = 0; i < testimages.size(); ++i)
    {
        //  if(i>=4)
        //      break;
        predictedLabel = model->predict(testimages[i]);
        imagetmp_rgb = showimages[i];
        cv::resize( imagetmp_rgb,  imagetmp_rgb, Size(120,120), 1, 1, CV_INTER_LINEAR);

        QImage image = QImage((const uchar*)imagetmp_rgb.data, imagetmp_rgb.cols, imagetmp_rgb.rows, QImage::Format_RGB888).rgbSwapped();
        labelpic[indexperson]->setPixmap(QPixmap::fromImage(image));
        // string result_message = format("Person = %d  ", predictedLabel);
        // labelname[indexperson]->setText(result_message.c_str());


        MYSQL mysql;
        mysql_init(&mysql);
        if(!mysql_real_connect(&mysql, "localhost", "dreamcity","304031870", "FaceDetRec", 3306, NULL, 0))
            {
                printf("failed\n");
             }
        else
        {
            MYSQL_RES *res_set;
            MYSQL_ROW row;
            string sql_select;
            sql_select=format("select Name from PeopleInfo WHERE IndexID=%d", predictedLabel);
            cout<<"sql_select111="<<sql_select.c_str()<<endl;
            mysql_query(&mysql, sql_select.c_str());


            res_set = mysql_store_result(&mysql);
            row = mysql_fetch_row(res_set);
            if(row) // data exist
            {
               string result_message = format("Person : %s  ", row[0]);
               labelname[indexperson]->setText(result_message.c_str());
            }
            mysql_close(&mysql);
        }

    }
    indexperson++;
    if( indexperson>4)
         indexperson=0;
}

// seevoid Detect::on_startCam_clicked()
void Recognize::on_startCAM_clicked()
{
    cap.open(0);
    if(!cap.isOpened())
    {
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
