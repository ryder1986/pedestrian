#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include "common.h"
#include "QDebug"
typedef struct camera_data{
    QString ip;
    int port;
}camera_data_t;
typedef struct data{
   int camera_amount;
   QList<camera_data_t> camera;
}data_t;
class Config : public QObject
{
    Q_OBJECT
public:
    data_t data;
    explicit Config(QObject *parent = 0);
     inline int get_int(QJsonObject obj,const char *member_name)
    {
        return obj[member_name].toInt();
    }
     inline QString get_string(QJsonObject obj,const char *member_name)
    {
        return obj[member_name].toString();
    }
     inline bool get_bool(QJsonObject obj,const char *member_name)
    {
        return obj[member_name].toBool();
    }
     inline QJsonObject get_child_obj(QJsonObject obj,const char *member_name)
    {
        return obj[member_name].toObject();
    }
     inline QJsonArray get_child_array(QJsonObject obj,const char *member_name)
    {
        return obj[member_name].toArray();
    }
     void parse_data()
     {
         QJsonObject root_obj=json_doc.object();
         data.camera_amount=get_int(root_obj,"camera_total_number");
         QJsonArray cams=get_child_array(root_obj,"camera");

                 foreach (QJsonValue v, cams) {
                     QJsonObject obj=v.toObject();
                     camera_data_t t;
                     t.ip=get_string(obj,"ip");
                     t.port=get_int(obj,"port");
                     data.camera.append(t);
                 }
         prt(info,"camera amount %d",data.camera_amount);
     }
     void set_data()
     {
           QJsonDocument json_doc_new;
           QJsonObject root_obj;

           root_obj["camera_total_number"]=data.camera_amount;
           QJsonArray cams;

           for(int i=0;i<data.camera_amount;i++)
           {
               QJsonObject o;
               o["ip"]=data.camera[i].ip;
               o["port"]=data.camera[i].port;
               cams.append(o);
           }
          // QJsonObject t;

           root_obj["camera"]=cams;
           json_doc_new.setObject(root_obj);
           json_doc=json_doc_new;
     }

    void load_config()
    {
        QFile *f=new QFile(config_filename);
        bool ret = f->open(QIODevice::ReadOnly);
        if(!ret){
             qDebug()<<config_filename;
             delete f;
        }
        QByteArray json_data;
        json_data=f->readAll();
        json_doc=QJsonDocument::fromJson(json_data);
        parse_data();
        f->close();
    }
    void save_config()
    {
        QFile *f=new QFile(config_filename);
        bool ret = f->open(QIODevice::WriteOnly);
        if(!ret){
             qDebug()<<config_filename;
             delete f;
        }
        QByteArray json_data;
        set_data();
        json_data=json_doc.toJson();
        f->write(json_data);
        f->close();
    }

signals:

public slots:
private:
    QString config_filename;
    QJsonDocument json_doc;
};

#endif // CONFIG_H
