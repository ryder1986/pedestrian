#ifndef SERVER_H
#define SERVER_H

#include <QObject>

#include <QObject>
#include <QTimer>
#include <QtNetwork>
#include <QNetworkInterface>
class Discover : public QObject{
    Q_OBJECT
    public:
    Discover(QObject *p=NULL){
        timer=new QTimer();
        connect(timer,SIGNAL(timeout()),this,SLOT(send_buffer()));
        udpSocket = new QUdpSocket(this);
        timer->start(1000);

    }
    ~Discover()
    {
        delete timer;
        delete udpSocket;

    }

 public  slots:
    void send_buffer()
    {
      //  qDebug()<<"time out ";
    //   datagram = "ip 192.168.1.216 ";
        qDebug()<<"sending buf";
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
                                 QHostAddress::Broadcast, 45454);
    }

private:
    QTimer *timer;
    int     messageNo;
    QByteArray datagram;
    QUdpSocket *udpSocket;
};
class tcpClient{
public:
    tcpClient(QTcpSocket *client_skt):skt(client_skt){

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
        connect(server, &QTcpServer::newConnection, this, &Server::new_client);

    }
    ~Server(){
        delete dis;
    }

signals:

public slots:
    void reply()
    {
        qDebug()<<"send one";
        QByteArray block;
//        QDataStream out(&block, QIODevice::WriteOnly);
//        out.setVersion(QDataStream::Qt_4_0);
//        out<<(quint16)0;
//        out << QString("123456789");
//        out.device()->seek(0);
//        out<<(quint16)(block.size()-sizeof(quint16));

        QString str("1234567890");
        block.append(str);
        QTcpSocket *skt = server->nextPendingConnection();
//        connect(skt, &QAbstractSocket::disconnected,
//                skt, &QObject::deleteLater);
        connect(skt, SIGNAL(disconnected()),
                skt, SLOT(deleteLater()));
        qDebug()<<"peer addr "<<skt->peerAddress()<<skt->peerPort();
        skt->write(block);
        skt->disconnectFromHost();
    }
    void new_client()
    {
        qDebug()<<"send one";
        QByteArray block;
        QString str("1234567890");
        block.append(str);
        QTcpSocket *skt = server->nextPendingConnection();
//        connect(skt, &QAbstractSocket::disconnected,
//                skt, &QObject::deleteLater);
        connect(skt, SIGNAL(disconnected()),
                skt, SLOT(deleteLater()));
        qDebug()<<"peer addr "<<skt->peerAddress()<<skt->peerPort();
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