#ifndef CLIENTMANUAL_H
#define CLIENTMANUAL_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
class ClientManual : public QObject
{
    Q_OBJECT
public:
    explicit ClientManual(QObject *parent = nullptr);
public slots:
    void init();
    void Uninit();
    void setIpPort(const QString &ip,const int &port);
    void startCheck();
private slots:
    void readData();
    void stateChanged(QAbstractSocket::SocketState socketState);
private:
    QTcpSocket *socket;
    QString hostIP;
    int m_port;
signals:
    void sig_constate(bool iscon);//连接人工站成功与否信号
    void sig_checkCompelate(uchar result);//人工检测完成，返回结果
};

#endif // CLIENTMANUAL_H
