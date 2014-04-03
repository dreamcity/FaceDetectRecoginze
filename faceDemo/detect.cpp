#include "detect.h"
#include "ui_detect.h"

using namespace std;
using namespace cv;

Detect::Detect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Detect)
{
    // the default construct function
    ui->setupUi(this);
    // set the firstLabel equal 0
    // set thee firstPerson equal 1;
    indexLabel=0;
    indexPerson=1;
    // always open the camera
    ON_OFF=true;
    // load the classifier , support by the offical opencv
    face_cascade_name = "haarcascade_frontalface_alt.xml";

    flag = false;

}

Detect::~Detect()
{
    delete ui;
}

// this function open the default camera of device
// and call detectFace() function  to detect the region of faces
// then show it on the textLabel
void Detect::on_startCam_clicked()
{
    // open the default camera , the parameter means the index of the camera device
    cap.open(-1);
    // initialize th person label with 1;
    ui->label_name->setText("Camera test!--Person:1");
    //always get the frame from video cap
    while(ON_OFF)
    {
        cap >> frame; // get a new frame from camera
        // Call the function detectFace,return a series rectangles, means faces
        detectFace(frame, faces);

        //before show the  face image, covert it into QImage
        // then show it on the textlabel
        QImage image = QImage((const uchar*)frame.data, frame.cols, frame.rows, QImage::Format_RGB888).rgbSwapped();
        ui->label_cam->setPixmap(QPixmap::fromImage(image));
        // keep function still working
        int c = waitKey(10);
        if( (char)c == 'c' )
            break;
    }
}

//this function add 1 to the indexPerson at every called;
// and clean the already exist labels
void Detect::on_changePerson_clicked()
{
    indexPerson++;
    string str0;
    str0=format("%d",  indexPerson);
    ui->label_name->setText(str0.c_str());

    QLabel *label[9]={ui->label1, ui->label2, ui->label3,
                      ui->label4, ui->label5, ui->label6,
                      ui->label7, ui->label8, ui->label9};

    // reassign the 9 labels
    // mean clean the previous-person's faces
    for(int i=0; i<9; i++)
    {
        string str1;
        str1=format("face%d", i);
        label[i]->setText(str1.c_str());
    }

    indexLabel=0;
}

// this function crop the face region in the camera_frame, but only support one face at once time
// resize the rectange region into 120*120
// pass the faces to labels
void Detect::on_takePic_clicked()
{
    // cap >> frame;
    // cout<<faces.size()<<endl;
    if(faces.size()==1)
    {
        Mat crop(frame, faces[0]);
        Mat cropface;
        // resize crop to cropface(120,120)
        cv::resize(crop, cropface, Size(120,120), 1, 1, CV_INTER_LINEAR);

        QImage image = QImage((const uchar*)cropface.data, cropface.cols, cropface.rows, QImage::Format_RGB888).rgbSwapped();
        QLabel *label[9]={ui->label1, ui->label2, ui->label3,
                          ui->label4, ui->label5, ui->label6,
                          ui->label7, ui->label8, ui->label9};
        label[indexLabel]->setPixmap(QPixmap::fromImage(image));

        // do the loop of 9 labels evaluate
        indexLabel++;
        if( indexLabel>8)
             indexLabel=0;
    }
    // if there are no face or more one face in the sight,error
    else
    {
        QMessageBox::critical(NULL, "critical", "Please make sure only one person in the camera!",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
 }

//realize the facedetect()
//input Mat
//output vector<Rect>
void Detect::detectFace(Mat frameface, vector<Rect>& faces)
{
    //before detectface , make sure load the classifier  successfully
    if( !face_cascade.load( face_cascade_name ) )
    {
        QMessageBox::critical(NULL, "critical", "--(!)Error loading!",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        exit(0);
    }
    if(flag && (imageROI.rows*imageROI.cols>22500))
    {
       frameface = imageROI; // get the ROI of face region
    }

    Mat frame_gray;
    // convert the origin RGB img into GRAY
    cvtColor( frameface, frame_gray, CV_BGR2GRAY );
    // equalize the gray img
    equalizeHist( frame_gray, frame_gray );

    // call detectMultiScale(), return a series rectange region means face;
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
    // use a rectange to get the face region
//    for( uint i = 0; i < faces.size(); i++ )
//    {
//        Point pt1(faces[i].x,faces[i].y);
//        Point pt2(faces[i].x+faces[i].width,faces[i].y+faces[i].height);
//        rectangle(frame,pt1,pt2,CV_RGB(0,255,0),3,2,0);
//    }

    // set the only one face in the sight, the bigest one!
   // if(faces.size())
    {
    uint indexFace =0;
    double RectSize = faces[indexFace].width*faces[indexFace].height;
    double tempSize;
    for( uint i = 0; i < faces.size(); i++ )
    {
        Point pt1(faces[i].x,faces[i].y);
        Point pt2(faces[i].x+faces[i].width,faces[i].y+faces[i].height);
        rectangle(frame,pt1,pt2,CV_RGB(0,255,0),3,2,0);

        tempSize = faces[i].width*faces[i].height;
        if(RectSize <= tempSize)
        {
            RectSize = tempSize;
            indexFace = i ;
        }
    }
    Rect imgface; // the probably region of face
    //imgface.x = faces[indexFace].x>50?(faces[indexFace].x-50):0;
    imgface.x = faces[indexFace].x*0.1;
 //   cout<<"imgface.x:"<<imgface.x<<endl;
    //imgface.y = faces[indexFace].y>50?(faces[indexFace].y-50):0;
    imgface.y = faces[indexFace].y*0.1;
 //   cout<<"imgface.y:"<<imgface.y<<endl;
    imgface.width = 350 - imgface.x;
//    cout<<"imgface.width:"<<imgface.width<<endl;
//    cout<<"faces[indexFace].width:"<<faces[indexFace].width<<endl;
    imgface.height = 350 - imgface.y;
//    cout<<"imgface.height:"<<imgface.height<<endl;
 //   cout<<"faces[indexFace].height:"<<faces[indexFace].height<<endl;
    //    if(imgface.x+1.5*faces[indexFace].width<400)
    //    {
    //        imgface.width = faces[indexFace].width *1.5;
    //    }
    //    else
    //    {
    //        imgface.width = 350 - imgface.x;
    //    }
    //    if(imgface.y+1.5*faces[indexFace].height<400)
    //    {
    //        imgface.height= faces[indexFace].height *1.5;
    //    }
    //    else
    //    {
    //        imgface.height = 350 - imgface.y;
    //    }
    imageROI = frame(cv::Rect(imgface)); // make the ROI region , serach this region at next time
    namedWindow("faceROIRegion");
    imshow("faceROIRegion", imageROI);
    flag = true; // flag , decide whether the face has been detected first time
    }
}


//save the pic into the flod of filepath
// which the index of filefold is the end of all folds
void Detect::on_savePic_clicked()
{
    //DIR *opendir(const char *name);
    //它返回一个DIR*类型，这就是一个句柄
    DIR *dp0;
    struct dirent *dirp;
        //    struct dirent *readdir(DIR *dir);
        //    该函数的返回值是struct dirent* 类型
        //    struct dirent {
        //                   ino_t          d_ino;       /* inode number */
        //                    off_t          d_off;       /* offset to the next dirent */
        //                    unsigned short d_reclen;    /* length of this record */
        //                    unsigned char  d_type;      /* type of file */
        //                    char           d_name[256]; /* filename */
        //    };
        //    这个结构体的d_name存放的就是文件的名字，这里的文件包括普通文件，目录文件等等，在linux的思想中，所有的东西都是文件。
    // open the "../facedata" if it exist,or make it
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

    // if there are same face data exist in the filepath, search them , and get the last fold ,add "../../imgXXX"
    // else there are no face data ,then create the "../../img001" directly

    if((dp1=opendir("../facedata/img001"))==NULL)
    {
        cout<<"No file is found"<<endl;
    }
    else
    {
        // search the whole fold, get the last fold
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
    // create the last number of ming floder name
    // such as ,if the ../img011 exist, the create ../img012 flod
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

    // save the pic of the assigned person into the assigned fold
    // the face image file names as "cropface1.jpg","cropface2.jpg","cropface3.jpg" ,etc.
    // ----./facedata/img001/cropface1.jpg
    // ---------------------/cropface2.jpg
    //----------------------..............
    // ----./facedata/img002/cropface1.jpg
    // ---------------------/cropface2.jpg
    //----------------------..............
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

    // save the person's information into mysqldatabase
    savedatabase();
    QMessageBox::information(NULL, "information", "file created successfully");
}

// using mysql++ , the interface of c++, connect mysql dabdabase, insert the person's infomation
void Detect::savedatabase()
{
    // cout<<"hello mysql"<<endl;
    MYSQL mysql;
    mysql_init(&mysql);

    //connect mysql database
    // servername: localhost
    // port: 3306 /default port
    // username: dreamcity
    // dadabasename: FaceDetRec
    // password: 304031870
    if(!mysql_real_connect(&mysql, "localhost", "dreamcity","304031870", "FaceDetRec", 3306, NULL, 0))
        {
             //  printf("failed\n");
            cout<<"failed to connect the database FaceDetRec"<<endl;
         }
    else
       {
            cout<<"Successed to connect the database FaceDetRec"<<endl;

            //declare a group of radioButton
            QButtonGroup BG;
            BG.addButton(ui->radioButton,0);
            BG.addButton(ui->radioButton_2,1);
            BG.addButton(ui->radioButton_3,2);
            int a = BG.checkedId();
            string sex;
            switch(a)
                {
                 case 0:
                      sex="male"; break;
                 case 1:
                      sex="female"; break;
                 case 2:
                     sex="unknown"; break;
                 default: break;
                 }
             // get the info of person
             string indexid;
             indexid=format("%d",  indexPerson);
             string name=ui->name->text().toStdString();
             string age=ui->age->text().toStdString();
             string phone=ui->phonenumber->text().toStdString();

             // seletct the database, whether the People exist
             string sql_select;
             sql_select=format("select IndexID from PeopleInfo WHERE IndexID=%d", indexPerson);
             // cout<<"sql_select="<<sql_select.c_str()<<endl;
             mysql_query(&mysql, sql_select.c_str());

             // the info of select operate
             // return a row of tables
             MYSQL_RES *res_set;
             MYSQL_ROW row;
             res_set = mysql_store_result(&mysql);
             row = mysql_fetch_row(res_set);
             //  cout<<"row="<<row<<endl;
             // if the data exist ,delete
             // this part of program, it can work better
             //  if exist
             //       { update the tables; }
             //  else
             //       { insert into the tables;}
             if(row) // data exist
             {
                //cout<<"row="<<row[0]<<endl;
                string sql_delete;
                sql_delete =format("delete from  PeopleInfo WHERE IndexID=%d", indexPerson);
                cout<<"sql_delete="<<sql_delete.c_str()<<endl;
                mysql_query(&mysql, sql_delete.c_str());
             }
             // insert the info into database;
             string sql_insert;
             sql_insert = "insert into PeopleInfo (IndexID,Name,Sex,Age,PhoneNumber) values (" +
                                      indexid + "," + "'" +
                                      name + "'" + "," + "'" +
                                      sex + "'" + "," +
                                      age + "," + "'" +
                                      phone + "'" + ")";
             cout<< "sql_insert="<<sql_insert.c_str()<<endl;
             mysql_query(&mysql, sql_insert.c_str());
             //close the connection of database;
             mysql_close(&mysql);
        }

}

//first .read the imgfilepath, make an config file to store the img and labels
//second call the 2dpca algorithm to train data
//return an config file to store the traindatas
void Detect::on_trainData_clicked()
{
    //search the imgfilepath, create img config dadafile
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

// first load the traindata file, "traindata.xml"
// call faceReg to relize the really function
void Detect::on_recognize_clicked()
{

    const char* traindatafile="../traindata.xml";
    // make sure the data is exist
    // before predict, must train first
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
        // predict each face in the camera
        for(uint i=0; i<faces.size(); i++)
        {
            //make sure the data is accuracy size, 120*120
            // gray, not RGB
            Mat crop(frame, faces[i]);
            cv::resize(crop, cropface_rgb, Size(120,120), 1, 1, CV_INTER_LINEAR);
            cvtColor( cropface_rgb, cropface_gray, CV_BGR2GRAY );
            equalizeHist(cropface_gray, cropface_gray );
            images_rgb.push_back(cropface_rgb);
            images_gray.push_back(cropface_gray);
            //call faceReg to predict gray_img ,and show rgb_face
            faceReg(traindatafile, images_rgb, images_gray);

        }
    }
}

// search the imgfilepath, create img config dadafile
// return imgconfig.txt
void Detect::createImgDataFile(const char *imgfilepath, const char *imgdatafile)
{
    // using the default file system ,rewrite the file everytimes
    imgDataFile.open(imgdatafile);
    //ergodic the assign filepath
    listDir(imgfilepath);
    imgDataFile.close();

}

//循环遍历
//递归 遍历子目录 ，以文件类型作为结束标识符，
//当对象是不是文件夹的时候，递归结束，
//同时在遍历每层子目录的时候，将文件的绝对路经以及对应文件夹的label写入配置文件
void Detect::listDir(const char *path)
{
    //see Detect::on_savePic_clicked()
    DIR              *pDir ;
    struct dirent    *ent  ;
    char              childpath[128];
    pDir=opendir(path);
    //  initialise
    memset(childpath,0,sizeof(childpath));
    while((ent=readdir(pDir))!=NULL)
    {
       // 判断是文件类型否为文件夹
       // 是文件夹
       if(ent->d_type & DT_DIR)
       {
           // exclude the fold "." and ".."
           if(strcmp(ent->d_name,".")==0 || strcmp(ent->d_name,"..")==0)
                continue;
           sprintf(childpath,"%s/%s",path,ent->d_name);
           // 递归调用 ，进入子目录
           listDir(childpath);
       }
       // 不是文件夹
       // 将文件的绝对路经以及对应文件夹的label写入配置文件
       else
       {

           int labelnum=0;
           int k=1;
           //path = ../facedata/img003, path[17]--path[15]=00x
           for(int i=17; i>14; i-- )
           {
               // char to int ,ascii
               labelnum+=(path[i]-'0')*k;
               k*=10;
           }
           //  cout<<path<<"/"<<ent->d_name<<";";
           //  cout<<labelnum<<endl;
           imgDataFile<<path<<"/"<<ent->d_name<<";";
           imgDataFile<<labelnum<<endl;

       }
    }
}

//使用CSV文件去读图像和标签，主要使用stringstream和getline方法
// read the imgconfig (.txt) file,
// return  vector<Mat>& images, vector<int>& labels
void Detect::read_csv(const string& filename, vector<Mat>& images, vector<int>& labels)
{
    // using ";" as separator to separator the img and label
    char separator =';';
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message ="No valid input file was given, please check the given filename.";
        QMessageBox::critical(NULL, "critical", error_message.c_str(),QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        CV_Error(CV_StsBadArg, error_message);
    }

    string line, path, classlabel;
    while (getline(file, line))
    {
        stringstream liness(line);
        //get line of file, end with separator,";"
        getline(liness, path, separator);
        //get line of file, end with "/0"
        getline(liness, classlabel);
        if(!path.empty()&&!classlabel.empty())
        {
            //push the images and labels into vector
            images.push_back(imread(path, 0));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
}

// train the data ,match images with labels
// first construct the base model of 2DPCA
// --2DPCA : public FaceRecognizer
// ----train();
// ----save();
// ----load();
// ----predict();
// then train it, get the traindatafile.xml
string Detect::faceTrain(std::vector<Mat> images,std::vector<int> labels)
{
    Ptr<FaceRecognizer> model = cv::createPCA2DFaceRecognizer(0);
    // call the PCA2DFaces::train()
    // using the 2DPCA AGL
    model->train(images, labels);

    string configfile("../traindata.xml");
    // call the FaceRecognizer::save() --> call the PCA2DFaces::save()
    // save the configfile.xml
    model->save(configfile);
    QMessageBox::information(NULL, "information", "The train is complete!");
    return configfile;

    //***********************************************************
    //    this part is work as EigenFace, using the PCA AGL,
    // support by the opencv offical! And it can work perfect
    //*******************
    //
    //    Ptr<FaceRecognizer> model = createEigenFaceRecognizer();
    //    model->train(images, labels);
    //    string configfile("../traindata.xml");
    //    model->save(configfile);
    //    // cout<<"The train is complete!"<<endl;
    //    QMessageBox::information(NULL, "information", "The train is complete!");
    //    return configfile;
    //*************************************************************
}

// predict the face capture by the camera
// first construct the base model of 2DPCA
// --2DPCA : public FaceRecognizer
// ----train();
// ----save();
// ----load();
// ----predict();
// second load the traindatafile.xml
// third predict the face ,get the label
// last select the label from database and show it on the textlabels
void Detect::faceReg(const string& configfile,std::vector<Mat> showimages, std::vector<Mat> testimages)
{
    Ptr<FaceRecognizer> model = createPCA2DFaceRecognizer(0);
    model->load(configfile);

    //  Ptr<FaceRecognizer> model = createEigenFaceRecognizer();
    //  model->load(configfile);

    int predictedLabel;
    Mat imagetmp_rgb;
    for (uint i = 0; i < testimages.size(); ++i)
    {
        // this time , it only support 4 people one time at most!
        if(i>=4)
            break;
        imagetmp_rgb = showimages[i];
        cv::resize( imagetmp_rgb,  imagetmp_rgb, Size(200,200), 1, 1, CV_INTER_LINEAR);
        // get the predictLabel of predict
        predictedLabel = model->predict(testimages[i]);

        //cout<<"The face"<<i<<" recognize is complete!"<<endl;
        // string result_message = format("Predicted Person = %d  ", predictedLabel);
        //QMessageBox::information(NULL, "information", result_message.c_str());

        // connect the mysql database , select from it and show
        // see savedatabase()
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
            if(row) // data exist
            {
             imshow(format("Person : %s  ", row[0]) , imagetmp_rgb);
            }
            mysql_close(&mysql);
        }
    }

}

