#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include"protocol.h"
#include <QObject>
#include <QTimer>
#include <QtNetwork>
#include <QNetworkInterface>
#include "common.h"
class Discover : public QObject{
    Q_OBJECT
public:
    Discover(QObject *p=NULL){
        timer=new QTimer();
        //     connect(timer,SIGNAL(timeout()),this,SLOT(send_buffer()));
        connect(timer,SIGNAL(timeout()),this,SLOT(check_client()));
        udpSocket = new QUdpSocket(this);
        udpSocket->bind(12346,QUdpSocket::ShareAddress);
        timer->start(1000);

    }
    ~Discover()
    {
        delete timer;
        delete udpSocket;

    }

public  slots:
    void check_client()
    {
        QByteArray client_msg;
        //    while(udpSocket->hasPendingDatagrams())
        if(udpSocket->hasPendingDatagrams())
        {
            client_msg.resize((udpSocket->pendingDatagramSize()));
            udpSocket->readDatagram(client_msg.data(),client_msg.size());
            qDebug()<<"get"<<client_msg.data()<<"from client";
            send_buffer_to_client();
            //   udpSocket->flush();
        }else{
            qDebug()<<"get nothing from client ";
        }
    }

    void send_buffer_to_client()
    {
        //  qDebug()<<"time out ";
        //   datagram = "ip 192.168.1.216 ";
        qDebug()<<"sending buf";
        QByteArray datagram;
        datagram.clear();
        QList <QNetworkInterface>list_interface=QNetworkInterface::allInterfaces();
        foreach (QNetworkInterface i, list_interface) {
            if(i.name()!="lo"){
                QList<QNetworkAddressEntry> list_entry=i.addressEntries();
                foreach (QNetworkAddressEntry e, list_entry) {
                    if(e.ip().protocol()==QAbstractSocket::IPv4Protocol)
                    {
                        //    qDebug()<<e.ip()<<e.netmask()<<e.broadcast();
                        datagram.append(QString(e.ip().toString())).append(QString(",")).\
                                append(QString(e.netmask().toString())).append(QString(",")).append(QString(e.broadcast().toString()));

                        //    qDebug()<<datagram;
                    }

                }
            }
        }
        udpSocket->writeDatagram(datagram.data(), datagram.size(),
                                 QHostAddress::Broadcast, 12347);
    }

private:
    QTimer *timer;
    QUdpSocket *udpSocket;
};

class tcpClient:QObject{
    Q_OBJECT
public:
    tcpClient(QTcpSocket *client_skt):skt(client_skt){
        //        connect(skt, SIGNAL(error(QAbstractSocket::SocketError)),
        //                //! [3]
        //                this, SLOT(displayError(QAbstractSocket::SocketError)));
        //       welcom_reply();
        qDebug()<<"conntected";
        connect(skt,SIGNAL(readyRead()),this,SLOT(real_reply()));
        connect(skt,SIGNAL(disconnected()),this,SLOT(deleteLater()));
    }
public slots:
    void simple_reply()
    {
        QByteArray client_buf=skt->readAll();

        QByteArray block;
        block.append(client_buf[0]+1);
        //        QString str("1234567890");
        //        block.append(str);
        skt->write(block);
        //  skt->disconnectFromHost();
    }
    void welcom_reply(){

        QByteArray block;
        QString str("welcom client ");
        block.append(str);
        skt->write(block);

    }
    void real_reply(){
        qDebug()<<"replying";
        QByteArray client_buf=skt->readAll();
        int cmd=Protocol::get_operation(client_buf.data());
        switch (cmd) {
        case ADD_CAMERA:
            prt(info,"protocol :add  cam");
            break;
        case GET_CONFIG:
            prt(info,"protocol :get config");
            break;
        default:
            break;
        }

    }

    void displayError(QAbstractSocket::SocketError socketError)
    {
        switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            qDebug()<<"error1"<<endl;
            break;
        case QAbstractSocket::ConnectionRefusedError:
            qDebug()<<"error2"<<endl;
            break;
        default:break;

        }


    }

private:
    QTcpSocket *skt;
};

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent=0 ):QObject(parent){
        dis=new Discover();
        bool ret=false;
        server=new QTcpServer();
        ret=server->listen(QHostAddress::Any,12345);
        if(ret){
            qDebug()<<"listen"<<server->serverPort();
        }else
        {
            qDebug()<<"listen fail";
        }
        //   connect(server, &QTcpServer::newConnection, this, &Server::reply);
        connect(server, &QTcpServer::newConnection, this, &Server::handle_incomimg_client);

    }
    ~Server(){
        delete dis;
    }

signals:

public slots:
    //    void reply()
    //    {
    //        qDebug()<<"send one";
    //        //        QByteArray block;
    //        ////        QDataStream out(&block, QIODevice::WriteOnly);
    //        ////        out.setVersion(QDataStream::Qt_4_0);
    //        ////        out<<(quint16)0;
    //        ////        out << QString("123456789");
    //        ////        out.device()->seek(0);
    //        ////        out<<(quint16)(block.size()-sizeof(quint16));

    //        //        QString str("1234567890");
    //        //        block.append(str);
    //        QTcpSocket *skt = server->nextPendingConnection();
    //        //        connect(skt, &QAbstractSocket::disconnected,
    //        //                skt, &QObject::deleteLater);
    //        connect(skt, SIGNAL(disconnected()),
    //                skt, SLOT(deleteLater()));
    //        qDebug()<<"peer addr "<<skt->peerAddress()<<skt->peerPort();
    //        //        skt->write(block);
    //        //        skt->disconnectFromHost();
    //    }
    void handle_incomimg_client()
    {
        QByteArray block;
        QString str("1234567890");
        block.append(str);
        QTcpSocket *skt = server->nextPendingConnection();
        //        connect(skt, &QAbstractSocket::disconnected,
        //                skt, &QObject::deleteLater);
        connect(skt, SIGNAL(disconnected()),
                skt, SLOT(deleteLater()));
        qDebug()<<"client addr incoming "<<skt->peerAddress()<<skt->peerPort();
        new tcpClient(skt);
        //    skt->write(block);
        //    skt->disconnectFromHost();
    }
    void handle_msg()
    {

    }

    void client_connected()
    {
        QTcpSocket *skt = server->nextPendingConnection();
        connect(skt, SIGNAL(disconnected()), skt, SLOT(deleteLater()));
        connect(skt,SIGNAL(readyRead()),this,SLOT(handle_msg));
    }
private:
    Discover *dis;
    QTcpServer *server;
};

#endif // SERVER_H
