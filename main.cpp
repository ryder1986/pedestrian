#include <QCoreApplication>
#include "common.h"
#include "config.h"
#include "camera.h"
#include "server.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
   // prt(info,"app start");

    //    for(int i=0;i<cfg.data.camera_amount;i++){
//    for(int i=0;i<1;i++){
//         Camera *c=new Camera(cfg.data.camera[i]);
//    }

    CameraManager *c=new CameraManager();
    Server *s=new Server();
    return a.exec();
}

