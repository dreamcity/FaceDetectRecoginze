#include "detect2recognize.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Detect2Recognize w;
    w.show();

    return a.exec();
}
