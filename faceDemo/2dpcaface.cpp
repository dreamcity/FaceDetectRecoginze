#include "2dpcaface.h"
using namespace cv;

//// Reads a sequence from a FileNode::SEQ with type _Tp into a result vector.




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

PCA2DFaces::PCA2DFaces()
{
}

PCA2DFaces::~PCA2DFaces()
{
}

PCA2DFaces::PCA2DFaces(int num_components )
{
   _num_components = num_components;
}

void PCA2DFaces::train(InputArrayOfArrays _src, InputArray _local_labels)
{
    
    // get the  numberof samples
    vector<Mat> images;
    Mat labels = _local_labels.getMat(); // vector<int> labels
    _src.getMatVector(images); // vector<Mat> images
//    _src.getMatVector();
    Mat image = images[images.size()-1];
    unsigned int numSamples=images.size();
    
    //MatrixXd DataPoints = image; // matrix (m x n)
    int m = image.rows;
    int n = image.cols;

    // clear existing model data
    _labels.release();
    _projections.clear();
    // clip number of components to be valid
    if((_num_components <= 0) || (_num_components > n))
        _num_components = n;

//    Mat DataSum ;  //add all the samples together
//    Mat DataMean ; // compute the mean of samples,eigen-face

    Mat DataSum = Mat::zeros(m, n, CV_32FC1);  //add all the samples together
    Mat DataMean = Mat::zeros(m, n, CV_32FC1); // compute the mean of samples,eigen-face
    for (unsigned int i =0; i<numSamples; ++i)
    {

        //cvConvertScale(images[i], images[i], 1/255.0);
        images[i].convertTo(images[i],CV_32F);
    }

    for (unsigned int i = 0; i < numSamples; ++i)
    {       
        DataSum += images[i];
    }
    DataMean = DataSum/(double)numSamples;
    _mean = DataMean.clone();
//    Mat Covariance;  // get the covariance matrix
//    Mat DataTemp; //matrix temp to get Gt = E[(A – EA)T (A – EA)]
    Mat Covariance= Mat::zeros(n, n, CV_32FC1);  // get the covariance matrix
    Mat DataTemp = Mat::zeros(m, n, CV_32FC1); //matrix temp to get Gt = E[(A – EA)T (A – EA)]
    for (unsigned int i = 0; i < numSamples; ++i)
    {
        DataTemp = images[i]-DataMean;
//        DataTemp.t();
       Covariance += DataTemp.t() * DataTemp;
         //Covariance +=  DataTemp.t().mul(DataTemp,1);
    }

    Covariance = (1 / (double) numSamples)*Covariance;
    
    
    // compute the eigenvalue on the Cov Matrix
    // *******************************************
    //Mat _eigenvectors

    eigen( Covariance, _eigenvalues, _eigenvectors );
    transpose(_eigenvectors, _eigenvectors); // eigenvectors by column
    _labels = labels.clone();

    //eigenvalues – output vector of eigenvalues of the same type as src;
    //the eigenvalues are stored in the descending order.

    //eigenvectors – output matrix of eigenvectors;
    //it has the same size and type as src; the
    //eigenvectors are stored as subsequent matrix rows,
    //in the same order as the corresponding  eigenvalues
    // *******************************************


    for(unsigned int sampleIdx = 0; sampleIdx < numSamples; sampleIdx++)
    {
        unsigned int dimSpace = _num_components; // num of components

        // choose the bigest dimSpace eigenvectors as the backproject matrix
        // i.e BackprojectMatrix X={X(0),X(1),...X(dimSpace)}
        // Y=AX, Y denote the matrix after backproject
        Mat ProjectMatrix = Mat::zeros(n, dimSpace, CV_32FC1);

        for (unsigned int i = 0; i < dimSpace; ++i)
        {
            ProjectMatrix.col(i) = _eigenvectors.col(i);
        }
        // X = ProjectData;
        Mat ProjectData = Mat::zeros(m, dimSpace, CV_32FC1);
        // Y = ProjectData;
        ProjectData = images[sampleIdx]*ProjectMatrix;
            
        _projections.push_back(ProjectData);
    }
}

void FaceRecognizer::save(const string& filename) const {
    FileStorage fs(filename, FileStorage::WRITE);
    if (!fs.isOpened())
        CV_Error(CV_StsError, "File can't be opened for writing!");
    this->save(fs);
    fs.release();
}
void PCA2DFaces::save(FileStorage& fs) const
{
    // write matrices
    fs << "num_components" << _num_components;
    fs << "mean" << _mean;
    fs << "eigenvalues" << _eigenvalues;
    fs << "eigenvectors" << _eigenvectors;
    // write sequences
    writeFileNodeList(fs, "projections", _projections);
    fs << "labels" << _labels;
}
void FaceRecognizer::load(const string& filename) {
    FileStorage fs(filename, FileStorage::READ);
    if (!fs.isOpened())
        CV_Error(CV_StsError, "File can't be opened for writing!");
    this->load(fs);
    fs.release();
}
void PCA2DFaces::load(const FileStorage& fs)
{
    //FileStorage fs(filename, FileStorage::READ);
    //read matrices
    fs["num_components"] >> _num_components;
    fs["mean"] >> _mean;
    fs["eigenvalues"] >> _eigenvalues;
    fs["eigenvectors"] >> _eigenvectors;
    // read sequences
    readFileNodeList(fs["projections"], _projections);
    fs["labels"] >> _labels;
}
void PCA2DFaces::predict(InputArray _src, int &minClass, double &minDist) const
{
}
int PCA2DFaces::predict(InputArray _src) const
{
    // load();

    Mat src = _src.getMat();
    src.convertTo(src,CV_32F);
    unsigned int m = src.rows;
    unsigned int n = src.cols;
    unsigned int dimSpace = _num_components; // num of components

    // choose the bigest dimSpace eigenvectors as the backproject matrix
    // i.e BackprojectMatrix X={X(0),X(1),...X(dimSpace)}
    // Y=AX, Y denote the matrix after backproject
    Mat ProjectMatrix = Mat::zeros(n, dimSpace, CV_32FC1);

    for (unsigned int i = 0; i < dimSpace; ++i)
    {
        ProjectMatrix.col(i) = _eigenvectors.col(i);
    }
    // X = ProjectData;
    Mat ProjectData = Mat::zeros(m, dimSpace, CV_32FC1);
    // Y = ProjectData;
    ProjectData = src*ProjectMatrix;
            
    int minClass = -1;
    double minDist = norm(_projections[0], ProjectData, NORM_L2);
    for(size_t sampleIdx = 0; sampleIdx < _projections.size(); sampleIdx++)
    {
        double dist = norm(_projections[sampleIdx], ProjectData, NORM_L2);
        if((dist <= minDist) )
        {
            minDist = dist;
            minClass = _labels.at<int>((int)sampleIdx);
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

