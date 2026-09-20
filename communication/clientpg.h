#ifndef CLIENTPG_H
#define CLIENTPG_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
class ClientPG : public QObject
{
    Q_OBJECT
public:
    explicit ClientPG(QObject *parent = nullptr);
public slots:
    void init();
    void Uninit();
    void setIpPort(const QString &ip,const int &port);
    void setParams(const QString &recipe,const QString &screenID);
    void getStressSignal(bool needOff);//PG上电下电  true时先下电再上电，false时直接上电
    void getPGammaSignal();//开始Pgamma

private slots:
    void readData();
    void stateChanged(QAbstractSocket::SocketState socketState);
private:
    QTcpSocket *socket;
    QString hostIP;
    int m_port;
    QString m_recipe;
    QString m_screenID;
    QByteArray recvBuffer;
    bool needOn = false;
    bool OnOrOff = false;//1:on 0:off

    void Initial();
    void PGPowerOn(bool on);
    void StartGamma();
    void processMessage(const QByteArray &msg);
signals:
    void sig_constate(bool iscon);//连接PG成功与否信号
    void sig_ToPLC_PGsuccess();//上电成功信号
    void sig_ToPLC_Pgammasuccess();//Pgamma完成信号
    void sig_LightUp(bool);//点亮信号
};

#endif // CLIENTPG_H
