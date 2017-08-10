#include <QCoreApplication>
#include "common.h"
#include "config.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
   // prt(info,"app start");
    Config cfg;
    return a.exec();
}

