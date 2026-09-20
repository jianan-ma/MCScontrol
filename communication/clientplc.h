#ifndef CLIENTPLC_H
#define CLIENTPLC_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
enum class STATION{
    PG_AND_PGAMMA,
    AOI,
    MANUAL,
    TP
};

class ClientPLC : public QObject
{
    Q_OBJECT
public:
    explicit ClientPLC(QObject *parent = nullptr);
public slots:
    void init();
    void Uninit();
    void setParams(uchar iNetNo,uchar iPLCNo,quint16 iIONo,uchar iModelNo,quint16 iOverTime);
    // 批量读取（按字）
    void batchRead(const QString &device, int startAddr, quint16 count);
    // 单字写入
    void singleRead(const QString &device, int addr);
    // 批量写入（按字）
    void batchWrite(const QString &device, int startAddr, const QVector<quint16> &values);
    // 单字写入
    void singleWrite(const QString &device, int addr, quint16 value);
    void requestStatus(STATION station);
    void relayData(int id,QByteArray ba);
    void completePG();//打开完成
    void completePgamma();//pgamma完成
    void completeAPI();//API完成
    void completeManual();//人工站完成
    void completeTP();//TP完成
private slots:
    void readData();
    void stateChanged(QAbstractSocket::SocketState socketState);
    void heartBeat();
private:
    QTcpSocket *socket;
    QTimer *timer;
    QString hostIP;
    int port;
    int requestAddr;
    int heartTimes=0;
    bool isRelay1 = false;
    bool isRelay2 = false;

    uchar netNo=0;//网络编号
    uchar PLCNo=0xff;//PLC编号
    quint16 IONo=0xff03;//IO编号
    uchar modelNo=0;//模块站号
    quint16 overTime=1;//等待PLC响应时间
private:
    QByteArray buildHeader(quint16 cmd, int dataLen);
    quint8 deviceCode(const QString &device);
    void sendFrame(const QByteArray &frame);
signals:
    void errorOccurred(QString);
    void sig_constate(bool iscon);
    void sig_crimpSignal();//压接信号
    void sig_PgammaSignal();//pgamma准备信号
    void sig_APISignal();//API开始信号
    void sig_manualStationSignal();//人工站到位信号
    void sig_positionSignal();//到位信号(TP页)
    void sig_sendReply(int id,QByteArray ba);//回复需转发给rose的数据
};

#endif // CLIENTPLC_H
