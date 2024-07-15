#include "dialog.h"
#include <QApplication>
#include"ckernel.h"
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    //Dialog w;
  //  w.show();
    Ckernel kernel;
    return a.exec();
}
