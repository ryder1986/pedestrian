#ifndef VIDEOSRC_H
#define VIDEOSRC_H

#include "videohandler.h"
class VideoSrc:public QObject{
    Q_OBJECT
public:

    VideoSrc()
    {
        //     p_cap= cvCreateFileCapture("rtsp://192.168.1.81:554");  //读取视频
        p_cap= cvCreateFileCapture("/root/repo-github/pedestrian/test.mp4");  //读取视频
    }
    VideoSrc(QString path)
    {
        //     p_cap= cvCreateFileCapture("rtsp://192.168.1.81:554");  //读取视频

        strcpy(url,path.toStdString().data());
        p_cap= cvCreateFileCapture(url);  //读取视频

        //    prt(info,"get %s",url.toStdString().data());
    }
    ~VideoSrc()
    {
        cvReleaseCapture(&p_cap);
        delete p_cap;
    }
    void set(VideoHandler &handler)
    {
        handler.frame_ori= cvQueryFrame(p_cap);
    }

    VideoHandler &operator>>(VideoHandler &handler)
    {

        int err=0;
        handler.frame_ori= cvQueryFrame(p_cap);
        if(handler.frame_ori==NULL){
            prt(info,"get video source fail, source url:%s",url);
            err=1;
            std::this_thread::sleep_for(chrono::milliseconds(1000));
        }else{
            //    prt(info,"get video source url:%s",url);
        }
        if(!err)
            handler.work(url);
        return handler;
    }
    // void operator +(int t)
    // {

    // }

    //    int  operator +(int i)
    //    {
    //       return i+1;
    //    }
    char *get_url(){
        return url;
    }

private:
    CvCapture *p_cap;
    char url[PATH_LEN];

};

#endif // VIDEOSRC_H
