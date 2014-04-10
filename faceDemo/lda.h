#ifndef LDA_H
#define LDA_H
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
using namespace std;
using namespace cv;
class LDAT
{
public:
    LDAT();
    ~LDAT();
    LDAT( int num);
    void lda(InputArrayOfArrays _src, InputArray _lbls) ;
    int getComponents();
    Mat getVectors();
    Mat getValues();
    Mat getMean();
    vector<Mat> getProjections();

private:
   int num_components;
   Mat eigenvectors;
   Mat eigenvalues;
   Mat meanTotal;
   vector<Mat> projections;
};

#endif // LDA_H
