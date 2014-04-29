#include "2dpcaface.h"
using namespace cv;
using namespace std;


PCA2DFaces::PCA2DFaces(int num_components )
{
   _num_components = num_components;
}
PCA2DFaces::PCA2DFaces()
{
}

PCA2DFaces::~PCA2DFaces()
{
}
// Reads a sequence from a FileNode::SEQ with type _Tp into a result vector.
template<typename _Tp>
inline void readFileNodeList(const FileNode& fn, vector<_Tp>& result) {
    if (fn.type() == FileNode::SEQ) {
        for (FileNodeIterator it = fn.begin(); it != fn.end();) {
            _Tp item;
            it >> item;
            result.push_back(item);
        }
    }
}

// Writes the a list of given items to a cv::FileStorage.
template<typename _Tp>
inline void writeFileNodeList(FileStorage& fs, const string& name,
                              const vector<_Tp>& items) {
    // typedefs
    typedef typename vector<_Tp>::const_iterator constVecIterator;
    // write the elements in item to fs
    fs << name << "[";
    for (constVecIterator it = items.begin(); it != items.end(); ++it) {
        fs << *it;
    }
    fs << "]";
}

//void PCA2DFaces::subproject(vector<Mat> images)
//{
//    for(unsigned int sampleIdx = 0; sampleIdx < numSamples; sampleIdx++)
//    {
//        unsigned int dimSpace = _num_components; // num of components

//        // choose the bigest dimSpace eigenvectors as the backproject matrix
//        // i.e BackprojectMatrix X={X(0),X(1),...X(dimSpace)}
//        // Y=AX, Y denote the matrix after backproject
//        Mat ProjectMatrix = Mat::zeros(n, dimSpace, CV_32FC1);
//        for (unsigned int i = 0; i < dimSpace; ++i)
//        {
//            _eigenvectors.col(i).copyTo(ProjectMatrix.col(i));
//        }
//        // X = ProjectData;
//        Mat ProjectData = Mat::zeros(n, dimSpace, CV_32FC1);
//        // Y = ProjectData;
//        ProjectData = images[sampleIdx]*ProjectMatrix;
//        // prepare to write in the .xml file
//        _projections.push_back(ProjectData);
//       // _labels.push_back(_labels)
//    }
//}
template<typename _Tp>
inline vector<_Tp> remove_dups(const vector<_Tp>& src) {
    typedef typename set<_Tp>::const_iterator constSetIterator;
    typedef typename vector<_Tp>::const_iterator constVecIterator;
    set<_Tp> set_elems;
    for (constVecIterator it = src.begin(); it != src.end(); ++it)
        set_elems.insert(*it);
    vector<_Tp> elems;
    for (constSetIterator it = set_elems.begin(); it != set_elems.end(); ++it)
        elems.push_back(*it);
    return elems;
}

void PCA2DFaces::train(InputArrayOfArrays _src, InputArray _local_labels)
{
    
    // get the  numberof samples
    vector<Mat> images;
    vector<int> labelclass;
    vector<Mat> projections;
    // vector<Mat> labelstemp;
    Mat labels = _local_labels.getMat(); // vector<int> labels


    std::vector<int> labelstmp;
    // safely copy the labels
    {
        Mat tmp = _local_labels.getMat();
        for(unsigned int i = 0; i < tmp.total(); i++)
        {
            labelstmp.push_back(tmp.at<int>(i));
        }
    }
    // number of unique labels
    int C = (int) remove_dups(labelstmp).size();
    cout<<"c:"<<C<<endl;
    //cout<<"labels.data:"<<labels.total()<<endl;
    _src.getMatVector(images); // vector<Mat> images

    unsigned int numSamples=images.size(); // the total number of all samples
    
    // matrix (m x n)
    Mat image = images[images.size()-1];
    int m = image.rows;
    int n = image.cols;

    //      _num_components= 30;
    // clip number of components to be valid
    if((_num_components <= 0) || (_num_components > C-1))
        _num_components = C-1;  // all the components

    Mat DataSum = Mat::zeros(m, n, CV_32FC1);  //add all the samples together
    Mat DataMean = Mat::zeros(m, n, CV_32FC1); // compute the mean of samples,eigen-face
    for (unsigned int i =0; i<numSamples; ++i)
    {
        //convert the orignial image CV_8UC1 to CV_32FC1
        images[i].convertTo(images[i],CV_32F);
    }
    // get the mean of all the samples data
    for (unsigned int i = 0; i < numSamples; ++i)
    {       
        DataSum += images[i];
    }
    DataMean = DataSum/(double)numSamples;
    _mean = DataMean.clone();

    Mat Covariance= Mat::zeros(n, n, CV_32FC1);  // get the covariance matrix
    Mat DataTemp = Mat::zeros(m, n, CV_32FC1); //matrix temp to get Gt = E[(A – EA)T (A – EA)]
    for (unsigned int i = 0; i < numSamples; ++i)
    {
       DataTemp = images[i]-DataMean;
       // add the matrix :E[(A – EA)T (A – EA)]
       Covariance += DataTemp.t() * DataTemp;
    }
    Covariance = (1 / (double) numSamples)*Covariance;
      
    // compute the eigenvalue on the Cov Matrix
    eigen( Covariance, _eigenvalues, _eigenvectors );
    transpose(_eigenvectors, _eigenvectors); // eigenvectors by column

    //_labels = labels.clone();

    // eigenvalues – output vector of eigenvalues of the same type as src;
    //   the eigenvalues are stored in the descending order.

    // eigenvectors – output matrix of eigenvectors;
    //   it has the same size and type as src; the eigenvectors are stored as subsequent matrix rows,
    //   in the same order as the corresponding  eigenvalues
    // clear existing model data
    _labels.release();
    _projections.clear();

    _labels = labels.clone();
    // subproject(images);
    // deal all the samples
    for(unsigned int sampleIdx = 0; sampleIdx < numSamples; sampleIdx++)
    {
        unsigned int dimSpace = numSamples-C; // num of components

        // choose the bigest dimSpace eigenvectors as the backproject matrix
        // i.e BackprojectMatrix X={X(0),X(1),...X(dimSpace)}
        // Y=AX, Y denote the matrix after backproject
        Mat ProjectMatrix = Mat::zeros(n, dimSpace, CV_32FC1);
        for (unsigned int i = 0; i < dimSpace; ++i)
        {
            _eigenvectors.col(i).copyTo(ProjectMatrix.col(i));
        }
        // X = ProjectData;
        Mat ProjectData = Mat::zeros(n, dimSpace, CV_32FC1);
        // Y = ProjectData;
        ProjectData = images[sampleIdx]*ProjectMatrix;
//        cout<<"ProjectData.row:"<<ProjectData.rows<<endl;
//        cout<<"ProjectData.col:"<<ProjectData.cols<<endl;
        // prepare to write in the .xml file
        projections.push_back(ProjectData);
       // _labels.push_back(_labels)
        labelclass.push_back(labels.at<int>(sampleIdx));
    }
    //***************************************************************
    // the components  shouble smaller than C-1,
    // but it doesnot work well
    // i set it equals numSamples-C;
    // so actually the lda means nothing
    _num_components = numSamples-C;
    //**************************************************************
    LDAT ldaAGL(_num_components);
    ldaAGL.lda(projections,labelclass);
    _num_pca_coms =  numSamples-C;
    _eigenvalues = ldaAGL.getValues();
    _eigenvectorslda = ldaAGL.getVectors();
    _mean = ldaAGL.getMean();
     cout<<"_mean:"<<_mean.rows<<endl;
     cout<<"_mean:"<<_mean.cols<<endl;
    _projections = ldaAGL.getProjections();

}

// FaceRecognizer::save(const string& filename) -- > PCA2DFaces::save(FileStorage& fs)
void FaceRecognizer::save(const string& filename) const
{
    FileStorage fs(filename, FileStorage::WRITE);
    if (!fs.isOpened())
        CV_Error(CV_StsError, "File can't be opened for writing!");
    this->save(fs);
    fs.release();
}
void PCA2DFaces::save(FileStorage& fs) const
{
    // write matrices
    fs << "num_pca_coms" << _num_pca_coms;
    fs << "num_components" << _num_components;
    fs << "mean" << _mean;
    fs << "eigenvalues" << _eigenvalues;
    fs << "eigenvectors" << _eigenvectors;
    fs << "eigenvectorslda"<< _eigenvectorslda;
    // write sequences
    writeFileNodeList(fs, "projections", _projections);
    fs << "labels" << _labels;
}

// FaceRecognizer::load(const string& filename) --> PCA2DFaces::load(const FileStorage& fs)
void FaceRecognizer::load(const string& filename)
{
    FileStorage fs(filename, FileStorage::READ);
    if (!fs.isOpened())
        CV_Error(CV_StsError, "File can't be opened for writing!");
    this->load(fs);
    fs.release();
}
void PCA2DFaces::load(const FileStorage& fs)
{
    //read matrices
    fs["num_pca_coms"] >>_num_pca_coms;
    fs["num_components"] >> _num_components;
    fs["mean"] >> _mean;
    fs["eigenvalues"] >> _eigenvalues;
    fs["eigenvectors"] >> _eigenvectors;
    fs["eigenvectorslda"] >> _eigenvectorslda;
    // read sequences
    readFileNodeList(fs["projections"], _projections);
    fs["labels"] >> _labels;
}

//*******************************************
// this function means nothing ,just make the PCA2DFaces  not to be an abstract class
// and why I do this ,because i don't want to see the "warning info ", shame on me!!!
void PCA2DFaces::predict(InputArray _src, int &minClass, double &minDist) const
{
    minClass = -1;
    minDist = 0; // nothing to use;
    predict(_src);
}
//******************************************

// this function is really to predict the testsample
// return the label ,int!
int PCA2DFaces::predict(InputArray _src) const
{
    Mat src = _src.getMat();
    src.convertTo(src,CV_32F);
    unsigned int m = src.rows;
    unsigned int n = src.cols;
    unsigned int dimSpace = _num_pca_coms; // num of components

    // choose the bigest dimSpace eigenvectors as the backproject matrix
    // i.e BackprojectMatrix X={X(0),X(1),...X(dimSpace)}
    // Y=AX, Y denote the matrix after backproject
    Mat ProjectMatrix = Mat::zeros(n, dimSpace, CV_32FC1);

    for (unsigned int i = 0; i < dimSpace; ++i)
    {
         _eigenvectors.col(i).copyTo(ProjectMatrix.col(i));
    }
    // X = ProjectData;
    Mat ProjectDatapca = Mat::zeros(m, dimSpace, CV_32FC1);
    // Y = ProjectData;
      ProjectDatapca = src*ProjectMatrix;
//    ProjectDatapca = src.mul(ProjectMatrix);

    // lda project
    unsigned int dim = _num_components;
    Mat ProjectMatrixlda = Mat::zeros(dimSpace, dim, CV_32FC1);
    for (unsigned int i = 0; i < dim; ++i)
    {
         _eigenvectorslda.col(i).copyTo(ProjectMatrixlda.col(i));
    }
    Mat ProjectData = Mat::zeros(m, dim, CV_32FC1);
    ProjectData = ProjectDatapca *ProjectMatrixlda;
    //ProjectData = ProjectDatapca.mul(ProjectMatrixlda);

    // initialize the default class equal 1
    int  minClass = _labels.at<int>((int)0);
    cout<<"minClass:"<<minClass<<endl;
    double minDist = norm(_projections[0], ProjectData, NORM_L2); // get the Euclidean distance
    cout<<"minDist:"<<minDist<<endl;
    // get the shortest distance and get the class label
    // it means predict result
    cout<<" _projections.size="<< _projections.size()<<endl;
    for(size_t sampleIdx = 0; sampleIdx < _projections.size(); sampleIdx++)
    {
        double dist = norm(_projections[sampleIdx], ProjectData, NORM_L2);
        cout<<"Dist:"<<sampleIdx<<":"<<dist<<endl;
//        minClass = _labels.at<int>((int)sampleIdx);
        cout<< "labels:"<<_labels.at<int>((int)sampleIdx)<<endl;
       // cout<<"minDist:"<<minDist<<endl;
        if((dist <= minDist) )
        {

            minDist = dist;
            cout<<"minDist:"<<minDist<<endl;
            minClass = _labels.at<int>((int)sampleIdx);
            cout<<"minClass:"<<minClass<<endl;
        }

    }
    return minClass;
}
namespace cv
{
    Ptr<FaceRecognizer> createPCA2DFaceRecognizer(int num_components)
    {
        return new PCA2DFaces(num_components);
    }
}

