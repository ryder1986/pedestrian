#ifndef CAMERA_H
#define CAMERA_H
/*
    well,there are two policies,one is emit buffer when VideoSrc avilable, one is timer emit fetching from VideoSrc per xx msecond.

*/
#include <QTimer>
#include "config.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <QObject>
using namespace cv;
using namespace std;
class CameraManager;
class VideoHandler: public QObject{
    Q_OBJECT
public:

    IplImage * frame_ori;
    VideoHandler()
    {

    }


    //    void operator>>(VideoHandler &handler)
    //   {
    //        //  handler.frame_ori= cvQ     return handler;
    //   }
    ~VideoHandler()
    {

    }
    void work(char *url)
    {
        int min_win_width = 64;	// 48, 64, 96, 128, 160, 192, 224
        int max_win_width = 256;

        CascadeClassifier cascade;
        vector<Rect> objs;
        //string cascade_name = "../Hog_Adaboost_Pedestrian_Detect\\hogcascade_pedestrians.xml";
        // string cascade_name = "/root/hogcascade_pedestrians.xml";
        string cascade_name = "/root/repo-github/pedestrian/hogcascade_pedestrians.xml";

        if (!cascade.load(cascade_name))
        {
            prt(info,"can't load cascade");
            // cout << "can't load cascade!" << endl;
            //return -1;
        }
#if 1

        // while (1)
        {
            //   frame_ori = cvQueryFrame(p_cap);
            //   frame.create(frame_ori->height,frame_ori->width,CV_8U);
            //   memcpy(frame.data,frame_ori->imageData,frame_ori->imageSize);
            Mat frame(frame_ori);
          //  imshow(url,frame);


            //  cout << "opened " << endl;
            //*p_cap >> frame;

            //        while(1)
            //            ;
            if (!frame.empty())
            {
                frame_num++;
                if (frame_num % 100 == 0)
                {
                    cout << "Processed " << frame_num << " frames!" << endl;
                }

                //   if (frame_num % 3 == 0)
                if (1)
                {
                    //resize(frame,frame,Size(frame.cols / 2, frame.rows / 2),CV_INTER_LINEAR);
                    //resize(frame,frame,Size(704, 576),CV_INTER_LINEAR);
                    cvtColor(frame, gray_frame, CV_BGR2GRAY);
                    //  gray_frame=frame;
                    //Rect rect;
                    //rect.x = 275;
                    //rect.y = 325;
                    //rect.width = 600;
                    //rect.height = 215;

                    //Mat detect_area = gray_frame(rect);
                    //cascade.detectMultiScale(detect_area,objs,1.1,3);
                    cascade.detectMultiScale(gray_frame, objs, 1.1, 3);


                    vector<Rect>::iterator it = objs.begin();
                    while (it != objs.end() && objs.size() != 0)
                    {
                        pedestrian_num++;
                        pedestrians = frame(*it);

                        Rect rct = *it;
                        if (rct.width >= min_win_width && rct.width < max_win_width)
                        {
                            //   sprintf(file_name, "%d.jpg", pedestrian_num);
                            //  imwrite(file_name, pedestrians);

                            //rct.x += rect.x;
                            //rct.y += rect.y;

                            rectangle(frame, rct, Scalar(0, 255, 0), 2);

                        }

                        it++;
                    }

                    //rectangle(frame,rect,Scalar(0,255,0),2);
                    //    imshow("result", frame);
                    //outputVideo << frame;
                    //   waitKey(1);
                    objs.clear();
                }

            }
            else
            {

                prt(info,"opencv handle frame error !");
                //break;
            }
        }
#endif
    }


private:
    Mat gray_frame;
    Mat pedestrians;
    int pedestrian_num = 0;
    int frame_num = 0;

};


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
//using namespace std;
class Camera : public QObject
{
    Q_OBJECT
public:
    explicit Camera(camera_data_t dat,QObject *parent=0) : data(dat),QObject(parent)
    {
        tick=0;
        p_video_src=new VideoSrc(data.ip);
        timer=new QTimer();
        connect(timer,SIGNAL(timeout()),this,SLOT(work()));
        timer->start(10);
    }
    ~Camera(){
        delete timer;
        delete p_video_src;
    }
    void restart(camera_data_t dat)
    {
        data=dat;
    }


signals:

public slots:
    void work()
    {

        *p_video_src>>video_handler;

        //        p_src->set(handler);
        //    handler.work();
        tick++;
       // char tmp[100];
//       QString tmp(p_video_src->get_url());
//        if(tick==20){
//        //   strcpy(tmp,p_video_src->url);
//            prt(info,"restart cam %s, per 20 frame",tmp.toStdString().data());
//            delete p_video_src;
//            p_video_src=new VideoSrc(tmp);
//            tick=0;
//        }
    }
private:
    camera_data_t data;
    QTimer *timer;
    VideoSrc*p_video_src;
    VideoHandler video_handler;
    int tick;
};

#include"camera.h"
class CameraManager:public QObject{
    Q_OBJECT
public:
    CameraManager(){
        p_cfg=new Config("/root/repo-github/pedestrian/config.json");
        for(int i=0;i<p_cfg->data.camera_amount;i++){
            Camera *c=new Camera(p_cfg->data.camera[i]);
            //   Camera c(cfg.data.camera[i]);
            cams.append(c);
        }
    }
    ~CameraManager(){
        for(int i=0;i<p_cfg->data.camera_amount;i++){
            delete cams[i];
        }
    }
public slots:
    void add_camera(QByteArray buf)
    {
        //         Camera *c=new Camera(cfg.data.camera[i]);
        //    p_cfg->data.camera_amount++;
        //camera_data_t ca;
        p_cfg->set_ba((buf));
        Camera *c=new Camera(p_cfg->data.camera[p_cfg->data.camera_amount-1]);
        cams.append(c);
    }
    void del_camera(int index)
    {

        p_cfg->data.camera.removeAt(index);
        p_cfg->data.camera_amount--;
        p_cfg->save_config_to_file();
        delete cams[index-1];
        cams.removeAt(index-1);

    }
    void modify_camera(int index)
    {
        cams[index]->restart(p_cfg->data.camera[index-1]);
    }
    int get_config(char *c)
    {
        //c= p_cfg->get_ba().data();

        char *src=p_cfg->get_ba().data();
        int len=p_cfg->get_ba().length();
        //   memcpy(c, p_cfg->get_ba().data(),p_cfg->get_ba().length());
        memcpy(c,src,len);

        return len;
    }

private:
    QList <Camera *> cams;
    //    Config cfg;
    Config *p_cfg;
};


#endif // CAMERA_H
