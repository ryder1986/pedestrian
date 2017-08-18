#include <QCoreApplication>
#include "common.h"
#include "config.h"
#include "camera.h"
#include "server.h"

int log_level=3;// 1.no log  2.print log 3 print and write log 4.write without print log
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server *s=new Server();
    return a.exec();
}

