#include "lda.h"
using namespace cv;
using namespace std;

LDAT::LDAT(int num )
{
   _num_components = num;
}

LDAT::LDAT()
{
}

LDAT::~LDAT()
{
}
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

int LDAT::getComponents()
{
    return  _num_components;
}

Mat LDAT::getVectors()
{
    return _eigenvectors;
}
Mat LDAT::getMean()
{
    return _meanTotal;
}

Mat LDAT::getValues()
{
    return _eigenvalues;
}



vector<Mat> LDAT::getProjections()
{
    return _projections;
}


void LDAT::lda(InputArrayOfArrays _src, InputArray _lbls)
{

    // Mat labelsmat = _lbls.getMat();
    std::vector<Mat> images;
    _src.getMatVector(images);
    unsigned int numSamples=images.size(); // the total number of all samples
    // matrix (m x n)
    Mat image = images[images.size()-1];
    int m = image.rows;
    int n = image.cols;
    cout<< "m:"<<m<<endl;
    cout<<"n:"<<n<<endl;

    std::vector<int> labels;
    // safely copy the labels
    {
        Mat tmp = _lbls.getMat();
        for(unsigned int i = 0; i < tmp.total(); i++)
        {
            labels.push_back(tmp.at<int>(i));
        }
    }
    // maps the labels, so they're ascending: [0,1,...,C]
    // at last mapped_labels[i]=[0,1,..C,0,1,..C,...]
    vector<int> mapped_labels(labels.size());
    vector<int> num2label = remove_dups(labels); //remove the duplicate(重复) element
    map<int, int> label2num;// 实际上并没有实现label与数目的一一对应
    for (int i = 0; i < (int)num2label.size(); i++)
        label2num[num2label[i]] = i;
    for (size_t i = 0; i < labels.size(); i++)
        mapped_labels[i] = label2num[labels[i]];

    // number of unique labels
    int C = (int)num2label.size();

    // we can't do a LDA on one class, what do you
    // want to separate from each other then?
    if(C == 1) {
        string error_message = "At least two classes are needed to perform a LDA. Reason: Only one class was given!";
        CV_Error(CV_StsBadArg, error_message);
    }

    // clip number of components to be a valid number
    //if ((_num_components <= 0) || (_num_components > (C - 1)))
    // {
    //    _num_components = (C - 1);
    // }

    Mat meanTotal = Mat::zeros(m, n, image.type());
    std::vector<Mat> meanClass(C);
    std::vector<int> numClass(C);

    // initialize
    for (int i = 0; i < C; i++)
    {
        numClass[i] = 0;
        meanClass[i] = Mat::zeros(m, n, image.type());
    }

    for (uint i = 0; i < numSamples; i++)
    {

        Mat instance = images[i];
        int classIdx = mapped_labels[i];
        add(meanTotal, instance, meanTotal);
        // get the every class total values
        add(meanClass[classIdx], instance, meanClass[classIdx]);
        // store the nums of each class
        numClass[classIdx]++;

        //next do the divide,such as meanClass[classIdx] = meanClass[classIdx] / numClass[classIdx]++
    }

    // calculate total mean
    meanTotal.convertTo(meanTotal, meanTotal.type(), 1.0 / static_cast<double> (numSamples));
    meanTotal.copyTo(_meanTotal);
    // calculate class means
    for (int i = 0; i < C; ++i)
    {
        meanClass[i].convertTo(meanClass[i], meanClass[i].type(), 1.0/static_cast<double> (numClass[i]));
    }

    // subtract class means
    // do the every sample subtract meanClass
    // because  within-classes scatter
    // Sw equal for i..C
    // ------------for j..numClass[ClassIdx], the num of each class
    // calculate within-classes scatter
    Mat Sw = Mat::zeros(n, n, image.type());
    for (uint i = 0; i < numSamples; ++i)
    {
        Mat tmp;
        int classIdx = mapped_labels[i];
        Mat instance = images[i];
        subtract(instance, meanClass[classIdx], instance); //instance = instance - meanClass[classIdx]

        mulTransposed(instance, tmp, true);// tmp = instance.t() * instance
        add(Sw, tmp, Sw);
        // Sw += instance*instance.t();
    }

    // calculate between-classes scatter
    Mat Sb = Mat::zeros(n, n, image.type());
    for (int i = 0; i < C; i++)
    {

        Mat tmp;
        subtract(meanClass[i], meanTotal, tmp);
        mulTransposed(tmp, tmp, true);
        tmp *= numClass[i];
        add(Sb, tmp, Sb);
    }

    // invert Sw 逆矩阵
    Mat Swi = Sw.inv();
    // M = inv(Sw)*Sb
    Mat M;
    gemm(Swi, Sb, 1.0, Mat(), 0.0, M);

    eigen( M, _eigenvalues, _eigenvectors );

    transpose(_eigenvectors, _eigenvectors); // _eigenvectors by column
    //  cout<<"_eigenvectors.rows"<<_eigenvectors.rows<<endl;
    //  cout<<"_eigenvectors.cols"<<_eigenvectors.cols<<endl;
    // _labels.release();
    // __projections.clear();

    // _labels = labelsmat.clone();
    // deal all the samples
    for(unsigned int sampleIdx = 0; sampleIdx < numSamples; sampleIdx++)
    {
        unsigned int dimSpace = _num_components; // num of components

        // choose the bigest dimSpace _eigenvectors as the backproject matrix
        // i.e BackprojectMatrix X={X(0),X(1),...X(dimSpace)}
        // Y=AX, Y denote the matrix after backproject
        Mat ProjectMatrix = Mat::zeros(n, dimSpace, image.type());
        for (unsigned int i = 0; i < dimSpace; ++i)
        {
            _eigenvectors.col(i).copyTo(ProjectMatrix.col(i));
        }
        // X = ProjectData;
        Mat ProjectData = Mat::zeros(n, dimSpace, image.type());
        // Y = ProjectData;
        ProjectData = images[sampleIdx]*ProjectMatrix;
        // prepare to write in the .xml file
        _projections.push_back(ProjectData);
       // _labels.push_back(_labels)
    }

}



