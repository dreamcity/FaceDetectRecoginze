//#include <iostream>
//#include "opencv2/opencv.hpp"
//#include "opencv2/core/core.hpp"
//#include "opencv2/contrib/contrib.hpp"
//#include "opencv2/objdetect/objdetect.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"

#include "lda.h"
using namespace std;
using namespace cv;
class PCA2DFaces : public FaceRecognizer

{
private:
    int _num_components; // the components od lda ,after the deal with pca
    int _num_pca_coms;  // the components of pca , set it as n-c
    double _threshold;
    vector<Mat> _projections; // the projection of lda, after pca + lda;
    Mat _labels;
    Mat _eigenvectors; // the eigenvectors of pca
    Mat _eigenvalues; // the eigenvalues of pca

    Mat _eigenvectorslda; // the eigenvector of lda;
    Mat _mean; // the totalmean of all samples, after lda;

public:
    using FaceRecognizer::save;
    using FaceRecognizer::load;

     PCA2DFaces();
    ~PCA2DFaces();
     PCA2DFaces( int num_components);

    // Computes an Eigenfaces model with images in src and corresponding labels
    void train(InputArrayOfArrays src, InputArray labels);

    void subproject(vector<Mat> images);
    // Predicts the label of a query image in src.
    int predict(InputArray src) const;

    void predict(InputArray _src, int &label, double &dist) const;
    // See FaceRecognizer::load.
    void load(const FileStorage& fs);

    // See FaceRecognizer::save.
    void save(FileStorage& fs) const;


    //AlgorithmInfo* info() const;
            
};

namespace cv
{
   // construct the class PCA2DFaces
   Ptr<FaceRecognizer> createPCA2DFaceRecognizer(int num_components =0);
}


