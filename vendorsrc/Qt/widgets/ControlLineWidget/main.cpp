#include <QtGui/QApplication>
#include "controllinewidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ControlLineWidget w;
    w.show();
    
    return a.exec();
}
