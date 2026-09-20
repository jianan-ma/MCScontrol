#ifndef SERVERTP_H
#define SERVERTP_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
class ServerTP : public QObject
{
    Q_OBJECT
public:
    explicit ServerTP(QObject *parent = nullptr);
public slots:
    void init();
    void Uninit();
    void sendTPTestsignal(int device);//TP测试信号 device设备号:1,2
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
    //TP测试完成0:NG 1:OK 2:非所有项目NG&包合短路断路NG 3:非所有项目NG & 包含固件类、CB Test类 NG
    void sig_result(int device,int result);
};

#endif // SERVERTP_H
