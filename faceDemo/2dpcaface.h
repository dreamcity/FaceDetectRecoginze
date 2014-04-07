#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;
class PCA2DFaces : public FaceRecognizer

{
private:
    int _num_components;
    double _threshold;
    vector<Mat> _projections;
//    vector<int> _labels;
    Mat _labels;
    Mat _eigenvectors;
    Mat _eigenvalues;
    Mat _mean;

public:
    using FaceRecognizer::save;
    using FaceRecognizer::load;

     PCA2DFaces();
    ~PCA2DFaces();
     PCA2DFaces( int num_components);

    // Computes an Eigenfaces model with images in src and corresponding labels
    void train(InputArrayOfArrays src, InputArray labels);
//    void train(InputArrayOfArrays _src, vector<int> _local_labels);
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


