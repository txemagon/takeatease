#include <QtGui/QApplication>
#include "controlboard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ControlBoard w;
    w.show();
    
    return a.exec();
}
