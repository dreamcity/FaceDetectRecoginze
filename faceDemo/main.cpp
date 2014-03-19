#include "detect2recognize.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Detect2Recognize w;
    // first construct a simple dialog with 3 buttons
    // "DataPrepare" -- detect.ui
    // "FaceRecognizer" -- recognize.ui
    // "close" -- close this dialog
    w.show();
    // a.exec() means the object work for-loops
    return a.exec();
}
