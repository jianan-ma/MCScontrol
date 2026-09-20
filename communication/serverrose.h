#ifndef SERVERROSE_H
#define SERVERROSE_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
class ServerRose : public QObject
{
    Q_OBJECT
public:
    explicit ServerRose(QObject *parent = nullptr);
public slots:
    void init();
    void Uninit();
    void getReply(int id,QByteArray ba);
private slots:
    void newConnection();
    void clientDisconnect();
    void readData1();
    void readData2();
private:
    QTcpServer *server;
    QTcpSocket *client1;
    QTcpSocket *client2;
    int port;
    QString dev1IP;
    QString dev2IP;
signals:
    void sig_devConnected(int device);//客户端已连接信号
    void sig_devDisConnected(int device);
    void sig_relay(int id,QByteArray ba);
};

#endif // SERVERROSE_H
