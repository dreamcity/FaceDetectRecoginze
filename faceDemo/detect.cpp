#include "detect.h"
#include "ui_detect.h"
#include "QMessageBox"
//#define MYSQLPP_MYSQL_HEADERS_BURIED
//#include <mysql++/mysql++.h>
//#include <QButtonGroup>
//#include <qbuttongroup.h>

using namespace cv;
Detect::Detect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Detect)
{
    ui->setupUi(this);
    indexLabel=0;
    indexPerson=1;
    ON_OFF=true;
    face_cascade_name = "haarcascade_frontalface_alt.xml";


}

Detect::~Detect()
{
    delete ui;
}

void Detect::on_startCam_clicked()
{
    cap.open(-1);
    ui->label_name->setText("Camera test!");
    while(ON_OFF)
    {
        cap >> frame; // get a new frame from camera
        detectFace(frame, faces);
        QImage image = QImage((const uchar*)frame.data, frame.cols, frame.rows, QImage::Format_RGB888).rgbSwapped();
        ui->label_cam->setPixmap(QPixmap::fromImage(image));
        int c = waitKey(10);
        if( (char)c == 'c' )
            break;
    }
}


void Detect::on_changePerson_clicked()
{
    indexPerson++;
    string str0;
    str0=format("%d",  indexPerson);
    ui->label_name->setText(str0.c_str());
    QLabel *label[9]={ui->label1, ui->label2, ui->label3,
                      ui->label4, ui->label5, ui->label6,
                      ui->label7, ui->label8, ui->label9};
    for(int i=0; i<9; i++)
    {
        string str1;
        str1=format("face%d", i);
        label[i]->setText(str1.c_str());
    }
    indexLabel=0;

}

void Detect::on_takePic_clicked()
{
//    cap >> frame;
//    cout<<faces.size()<<endl;
    if(faces.size()==1)
    {
        Mat crop(frame, faces[0]);
        Mat cropface;
        cv::resize(crop, cropface, Size(120,120), 1, 1, CV_INTER_LINEAR);
        QImage image = QImage((const uchar*)cropface.data, cropface.cols, cropface.rows, QImage::Format_RGB888).rgbSwapped();
        QLabel *label[9]={ui->label1, ui->label2, ui->label3,
                          ui->label4, ui->label5, ui->label6,
                          ui->label7, ui->label8, ui->label9};
        label[indexLabel]->setPixmap(QPixmap::fromImage(image));
        indexLabel++;
        if( indexLabel>8)
             indexLabel=0;
    }
    else
    {
        QMessageBox::critical(NULL, "critical", "Please make sure only one person in the camera!",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
 }

void Detect::detectFace(Mat frame, vector<Rect>& faces)
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



void Detect::on_savePic_clicked()
{
    DIR *dp0;
    struct dirent *dirp;

    if((dp0=opendir("../facedata"))==NULL)
        {
            mkdir("../facedata", 0777);
            cout <<"filefolder was successfully created"<<endl;
        }
    else
        {
            cout <<"filefolder was exist"<<endl;
        }
    DIR *dp1;
    if((dp1=opendir("../facedata/img001"))==NULL)
    {
        cout<<"No file is found"<<endl;
    }
    else
    {
        while ((dirp=readdir(dp0))!=NULL)
        {
            if (dirp->d_name[0]=='i')
            {
                int temp=0;
                int k=1;
                for(int j=5; j>=3; j--)
                {
                    temp+=(dirp->d_name[j]-'0')*k;
                    k*=10;
                }
                indexPerson=(indexPerson>=temp?indexPerson:temp);
//                cout<<"indexperson="<<indexPerson<<endl;
            }
        }

    }
    string imgpath("../facedata/");
    string imgname("img000");
    int tempindex=indexPerson;
    for(int t=5; t>=3; t--)
    {
        imgname[t]+=tempindex%10;
        tempindex=tempindex/10;
    }
    imgpath=imgpath+imgname;
    mkdir(imgpath.c_str(),0777);


    const QPixmap *pixmap[9]={ui->label1->pixmap(), ui->label2->pixmap(), ui->label3->pixmap(),
                              ui->label4->pixmap(), ui->label5->pixmap(), ui->label6->pixmap(),
                              ui->label7->pixmap(), ui->label8->pixmap(), ui->label9->pixmap()
                             };
    string filepath;
    for(int i=0; i<9; i++)
    {
        if(pixmap[i])
        {
            string picpath(imgpath);
            filepath=format("/cropface%d.png", i);
            picpath+=filepath;
             pixmap[i]->save(picpath.c_str());
        }
    }
    savedatabase();
    QMessageBox::information(NULL, "information", "file created successfully");
}

void Detect::savedatabase()
{
    cout<<"hello mysql"<<endl;
    MYSQL mysql;
    mysql_init(&mysql);

    cout<<"hello mysql again"<<endl;
    if(!mysql_real_connect(&mysql, "localhost", "dreamcity","304031870", "FaceDetRec", 3306, NULL, 0))
        {
            printf("failed\n");
         }
    else
       {
        printf("success\n");
        QButtonGroup BG;
        BG.addButton(ui->radioButton,0);
        BG.addButton(ui->radioButton_2,1);
        BG.addButton(ui->radioButton_3,2);
        int a = BG.checkedId();
        string sex;
        switch(a)
            {
             case 0:
                  sex="male";
                  break;
             case 1:
                  sex="female";
                 break;
            case 2:
                 sex="unknown";
                break;
             default:
                 break;
             }
         string indexid;
         indexid=format("%d",  indexPerson);
         string name=ui->name->text().toStdString();
         string age=ui->age->text().toStdString();
         string phone=ui->phonenumber->text().toStdString();
//****************************************************************************
         string sql_select;
         sql_select=format("select IndexID from PeopleInfo WHERE IndexID=%d", indexPerson);
         cout<<"sql_select="<<sql_select.c_str()<<endl;
         mysql_query(&mysql, sql_select.c_str());

         MYSQL_RES *res_set;
         MYSQL_ROW row;
         res_set = mysql_store_result(&mysql);
         row = mysql_fetch_row(res_set);
      //  cout<<"row="<<row<<endl;
         if(row) // data exist
         {
            cout<<"row="<<row[0]<<endl;
            string sql_delete;
            sql_delete =format("delete from  PeopleInfo WHERE IndexID=%d", indexPerson);
            cout<<"sql_delete="<<sql_delete.c_str()<<endl;
            mysql_query(&mysql, sql_delete.c_str());
         }
         else //can not find
         {
             cout<<"helloworld"<<endl;
         }
         string sql_insert;
         sql_insert = "insert into PeopleInfo (IndexID,Name,Sex,Age,PhoneNumber) values (" +
                                  indexid + "," + "'" +
                                  name + "'" + "," + "'" +
                                  sex + "'" + "," +
                                  age + "," + "'" +
                                  phone + "'" + ")";
         cout<< "sql_insert="<<sql_insert.c_str()<<endl;
         mysql_query(&mysql, sql_insert.c_str());
        mysql_close(&mysql);
    }
}

void Detect::on_trainData_clicked()
{
    //create img config dadafile
    //----imgcofig.txt
    //----../facedata/img001/cropface0;1
    const char* imgfilepath="../facedata";
    const char* imgdatafile="../imgconfig.txt";
    createImgDataFile(imgfilepath, imgdatafile);
    //get the images and the labels in the imgcofig.txt
    //put them in the vector<Mat> and vector<int>
    vector<Mat> images;
    vector<int> labels;
    read_csv(imgdatafile, images, labels);
    // train the images with labels
    //put them in the  trainconfigfile;
    trainconfigfile=faceTrain(images,labels);

}
void Detect::on_recognize_clicked()
{

    const char* traindatafile="../traindata.xml";
    if((access(traindatafile,F_OK))==-1)
    {
          QMessageBox::critical(NULL, "critical", "NO Train data exit! Please train first!",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    else
    {
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

void Detect::createImgDataFile(const char *imgfilepath, const char *imgdatafile)
{
    imgDataFile.open(imgdatafile);
    listDir(imgfilepath);
    imgDataFile.close();

}

void Detect::listDir(const char *path)
{
    DIR              *pDir ;
    struct dirent    *ent  ;
    char              childpath[128];
    pDir=opendir(path);
    memset(childpath,0,sizeof(childpath));
    while((ent=readdir(pDir))!=NULL)
    {
       if(ent->d_type & DT_DIR)
       {
           if(strcmp(ent->d_name,".")==0 || strcmp(ent->d_name,"..")==0)
                continue;
           sprintf(childpath,"%s/%s",path,ent->d_name);
           listDir(childpath);
       }
       else
       {

           int labelnum=0;
           int k=1;
           //path = ../facedata/img003, path[17]--path[15]=00x
           for(int i=17; i>14; i-- )
           {
               labelnum+=(path[i]-'0')*k;
               k*=10;
           }
//           cout<<path<<"/"<<ent->d_name<<";";
//           cout<<labelnum<<endl;
           imgDataFile<<path<<"/"<<ent->d_name<<";";
           imgDataFile<<labelnum<<endl;

       }
    }
}

void Detect::read_csv(const string& filename, vector<Mat>& images, vector<int>& labels)
{
    char separator =';';
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message ="No valid input file was given, please check the given filename.";
        QMessageBox::critical(NULL, "critical", error_message.c_str(),QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        CV_Error(CV_StsBadArg, error_message);
    }
    string line, path, classlabel;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty()&&!classlabel.empty()) {
            images.push_back(imread(path, 0));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
}

string Detect::faceTrain(std::vector<Mat> images,std::vector<int> labels)
{

    Ptr<FaceRecognizer> model = createEigenFaceRecognizer();
    model->train(images, labels);
    string configfile("../traindata.xml");
    model->save(configfile);
//    cout<<"The train is complete!"<<endl;
    QMessageBox::information(NULL, "information", "The train is complete!");
    return configfile;
}

void Detect::faceReg(const string& configfile,std::vector<Mat> showimages, std::vector<Mat> testimages)
{


    Ptr<FaceRecognizer> model = createEigenFaceRecognizer();
    model->load(configfile);

    int predictedLabel;
    Mat imagetmp_rgb;
    for (uint i = 0; i < testimages.size(); ++i)
    {
        if(i>=4)
            break;
        imagetmp_rgb = showimages[i];
        cv::resize( imagetmp_rgb,  imagetmp_rgb, Size(200,200), 1, 1, CV_INTER_LINEAR);
        predictedLabel = model->predict(testimages[i]);
        //cout<<"The face"<<i<<" recognize is complete!"<<endl;
//        string result_message = format("Predicted Person = %d  ", predictedLabel);
        //QMessageBox::information(NULL, "information", result_message.c_str());
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
            cout<<"sql_select000="<<sql_select.c_str()<<endl;
            mysql_query(&mysql, sql_select.c_str());
            res_set = mysql_store_result(&mysql);
            row = mysql_fetch_row(res_set);
        //    cout<<"row="<<row<<endl;
            if(row) // data exist
            {
             imshow(format("Person : %s  ", row[0]) , imagetmp_rgb);
            }
            mysql_close(&mysql);
        }
    }


}

void Detect::on_pushButton_2_clicked()
{

    cout<<"hello mysql"<<endl;
    MYSQL mysql;
    mysql_init(&mysql);

    cout<<"hello mysql again"<<endl;
    if(!mysql_real_connect(&mysql, "localhost", "dreamcity","304031870", "FaceDetRec", 3306, NULL, 0))
        {
            printf("failed\n");
         }
    else
       {
        printf("success\n");
        QButtonGroup BG;
        BG.addButton(ui->radioButton,0);
        BG.addButton(ui->radioButton_2,1);
        BG.addButton(ui->radioButton_3,2);
        int a = BG.checkedId();
        string sex;
        switch(a)
            {
             case 0:
                  sex="male";
                  break;
             case 1:
                  sex="female";
                 break;
            case 2:
                 sex="unknown";
                break;
             default:
                 break;
             }
         string indexid;
         indexid=format("%d",  indexPerson);
         string name=ui->name->text().toStdString();
         string age=ui->age->text().toStdString();
         string phone=ui->phonenumber->text().toStdString();
//****************************************************************************
         string sql_select;
         sql_select=format("select IndexID from PeopleInfo WHERE IndexID=%d", indexPerson);
         cout<<"sql_select="<<sql_select.c_str()<<endl;
         mysql_query(&mysql, sql_select.c_str());

         MYSQL_RES *res_set;
         MYSQL_ROW row;
         res_set = mysql_store_result(&mysql);
         row = mysql_fetch_row(res_set);
      //  cout<<"row="<<row<<endl;
         if(row) // data exist
         {
            cout<<"row="<<row[0]<<endl;
            string sql_delete;
            sql_delete =format("delete from  PeopleInfo WHERE IndexID=%d", indexPerson);
            cout<<"sql_delete="<<sql_delete.c_str()<<endl;
            mysql_query(&mysql, sql_delete.c_str());
         }
         else //can not find
         {
             cout<<"helloworld"<<endl;
         }
         string sql_insert;
         sql_insert = "insert into PeopleInfo (IndexID,Name,Sex,Age,PhoneNumber) values (" +
                                  indexid + "," + "'" +
                                  name + "'" + "," + "'" +
                                  sex + "'" + "," +
                                  age + "," + "'" +
                                  phone + "'" + ")";
         cout<< "sql_insert="<<sql_insert.c_str()<<endl;
         mysql_query(&mysql, sql_insert.c_str());
        mysql_close(&mysql);
//        cout<<"res_set="<<res_set<<endl;
//         result = mysql_query(&mysql, "select IndexID from PeopleInfo WHERE IndexID=3");
//         cout<<"result="<<result<<endl;






//*****************************************************************************
//         string sql_delete;
//         sql_delete =format("delete from  PeopleInfo WHERE IndexID=%d", indexPerson);
//         cout<<"sql_delete="<<sql_delete.c_str()<<endl;
//         mysql_query(&mysql, sql_delete.c_str());
//         string sql_insert;
//         sql_insert = "insert into PeopleInfo (IndexID,Name,Sex,Age,PhoneNumber) values (" +
//                         indexid + "," + "'" +
//                         name + "'" + "," + "'" +
//                         sex + "'" + "," +
//                         age + "," + "'" +
//                         phone + "'" + ")";
//         cout<< "sql_insert="<<sql_insert.c_str()<<endl;
//         mysql_query(&mysql, sql_insert.c_str());
//         mysql_close(&mysql);
//********************************************************************************
//         cout<<"indexid="<<indexid<<endl;
//         string sql_select;
//         sql_select=format("select IndexID from PeopleInfo WHERE IndexID=%d", indexPerson);
//         cout<<"sql_select="<<sql_select.c_str()<<endl;
//        mysql_query(&mysql, sql_select.c_str());
//        MYSQL_RES *res_set;
//        MYSQL_ROW row;
//        res_set = mysql_store_result(&mysql);
//        row = mysql_fetch_row(res_set);
//        cout<<"row="<<row[0]<<endl;
//        cout<<"res_set="<<res_set<<endl;
//         result = mysql_query(&mysql, "select IndexID from PeopleInfo WHERE IndexID=3");
//         cout<<"result="<<result<<endl;
//         if(result)
//         {

//            string sql_delete;
//            sql_delete =format("delete from  PeopleInfo WHERE IndexID=%d", indexPerson);
//            cout<<"sql_delete="<<sql_delete.c_str()<<endl;
//            mysql_query(&mysql, sql_delete.c_str());


//         }
//       //  else
//         {
//             string sql_insert;
//             sql_insert = "insert into PeopleInfo (IndexID,Name,Sex,Age,PhoneNumber) values (" +
//                             indexid + "," + "'" +
//                             name + "'" + "," + "'" +
//                             sex + "'" + "," +
//                             age + "," + "'" +
//                             phone + "'" + ")";
//             cout<< "sql_insert="<<sql_insert.c_str()<<endl;

//             mysql_query(&mysql, sql_insert.c_str());
//         }
//         mysql_close(&mysql);

       }
}
