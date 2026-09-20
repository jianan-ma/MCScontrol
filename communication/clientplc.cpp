#include "clientplc.h"
#include <QSettings>
#include <QDataStream>

ClientPLC::ClientPLC(QObject *parent)
    : QObject{parent}
{
    // QSettings setini("config.ini",QSettings::IniFormat);
    // hostIP = setini.value("PLC/ServerIP").toString();
    // port = setini.value("PLC/port").toInt();
    hostIP = "192.168.0.10";
    port = 5000;
}

void ClientPLC::init()
{
    socket = new QTcpSocket(this);
    connect(socket,&QTcpSocket::readyRead,this,&ClientPLC::readData);
    connect(socket,&QTcpSocket::stateChanged,this,&ClientPLC::stateChanged);
    socket->connectToHost(hostIP,port);
}

void ClientPLC::Uninit()
{
    socket->close();
}

void ClientPLC::setParams(uchar iNetNo, uchar iPLCNo, quint16 iIONo, uchar iModelNo, quint16 iOverTime)
{
    netNo = iNetNo;
    PLCNo = iPLCNo;
    IONo = iIONo;
    modelNo = iModelNo;
    overTime = iOverTime;
}

void ClientPLC::batchRead(const QString &device, int startAddr, quint16 count)
{
    requestAddr = startAddr;
    QByteArray frame = buildHeader(0x0401, 0x0C00);
    quint8 addrL = startAddr & 0xff;
    quint8 addrM = (startAddr >> 8) & 0xff;
    quint8 addrH = (startAddr >>16) & 0xff;
    quint8 devcode = deviceCode(device);
    frame.append(addrL);
    frame.append(addrM);
    frame.append(addrH);
    frame.append(devcode);
    quint8 countL = count&0xff;
    quint8 countH = count>>8;
    frame.append(countL);
    frame.append(countH);
    sendFrame(frame);
}

void ClientPLC::singleRead(const QString &device, int addr)
{
    requestAddr = addr;
    QByteArray frame = buildHeader(0x0401, 0x0C00);
    quint8 addrL = addr & 0xff;
    quint8 addrM = (addr >> 8) & 0xff;
    quint8 addrH = (addr >>16) & 0xff;
    quint8 devcode = deviceCode(device);
    frame.append(addrL);
    frame.append(addrM);
    frame.append(addrH);
    frame.append(devcode);
    frame.append(quint8(1));
    frame.append(quint8(0));
    sendFrame(frame);
}

void ClientPLC::batchWrite(const QString &device, int startAddr, const QVector<quint16> &values)
{
    requestAddr = startAddr;
    quint16 dataLen = 0x000C + values.size() * 2;
    quint16 anti_len = ((dataLen&0xff)<<8)|(dataLen>>8);
    QByteArray frame = buildHeader(0x1401, anti_len);
    quint8 addrL = startAddr & 0xff;
    quint8 addrM = (startAddr >> 8) & 0xff;
    quint8 addrH = (startAddr >>16) & 0xff;
    quint8 devcode = deviceCode(device);
    frame.append(addrL);
    frame.append(addrM);
    frame.append(addrH);
    frame.append(devcode);
    quint16 count = values.count();
    quint8 countL = count&0xff;
    quint8 countH = count>>8;
    frame.append(countL);
    frame.append(countH);
    for(quint16 i=0;i<count;i++){
        quint16 value = values.at(i);
        quint8 valueL = value&0xff;
        quint8 valueH = value>>8;
        frame.append(valueL);
        frame.append(valueH);
    }
    sendFrame(frame);
}

void ClientPLC::singleWrite(const QString &device, int addr, quint16 value)
{
    requestAddr = addr;
    QByteArray frame = buildHeader(0x1401, 0x0E00);
    quint8 addrL = addr & 0xff;
    quint8 addrM = (addr >> 8) & 0xff;
    quint8 addrH = (addr >>16) & 0xff;
    quint8 devcode = deviceCode(device);
    frame.append(addrL);
    frame.append(addrM);
    frame.append(addrH);
    frame.append(devcode);
    frame.append(quint8(1));
    frame.append(quint8(0));
    quint8 valueL = value&0xff;
    quint8 valueH = value>>8;
    frame.append(valueL);
    frame.append(valueH);
    sendFrame(frame);
}

void ClientPLC::requestStatus(STATION station)
{
    switch (station) {
    case STATION::PG_AND_PGAMMA:
        singleRead("D",16020);
        break;
    case STATION::AOI:
        singleRead("D",16210);
        break;
    case STATION::MANUAL:
        singleRead("D",16410);
        break;
    case STATION::TP:
        singleRead("D",16610);
        break;
    }
}

void ClientPLC::relayData(int id,QByteArray ba)
{
    if(id==1)
        isRelay1 = true;
    else
        isRelay2 = true;
    sendFrame(ba);
}

void ClientPLC::completePG()
{
    singleWrite("D",16020,1);//不确定
}

void ClientPLC::completePgamma()
{
    singleWrite("D",16020,5);
}

void ClientPLC::completeAPI()
{
    singleWrite("D",16210,5);
}

void ClientPLC::completeManual()
{
    singleWrite("D",16410,5);
}

void ClientPLC::completeTP()
{
    singleWrite("D",16610,5);
}

void ClientPLC::readData()
{
    QByteArray ba = socket->readAll();
    qDebug()<<ba.toHex();
    if(ba.length()<11)
        return;
    if(isRelay1){
        isRelay1 = false;
        emit sig_sendReply(1,ba);
        return;
    }
    if(isRelay2){
        isRelay2 = false;
        emit sig_sendReply(2,ba);
        return;
    }
    if(uchar(ba[0])==0xd0 && uchar(ba[1])==0 && uchar(ba[9])==0 && uchar(ba[10])==0){
        if(ba.length()==13){
            quint16 value = (uchar(ba[12])<<8)|uchar(ba[11]);
            switch (requestAddr) {
            // 后续可能需要自己做心跳
            // case 16000:
            //     if(value==0){
            //         singleWrite("D",16000,1);
            //     }
            //     else{
            //         heartTimes++;
            //         if(heartTimes>=5){
            //             emit sig_constate(false);
            //         }
            //     }
            //     break;
            case 16020:
                if(value==0)//此处不确定，后续更改
                    emit sig_crimpSignal();
                else if(value==1)
                    emit sig_PgammaSignal();
                break;
            case 16210:
                if(value==1)//不确定
                    emit sig_APISignal();
                break;
            case 16410:
                if(value==1)//不确定
                    emit sig_manualStationSignal();
                break;
            case 16610:
                if(value==1)//不确定
                    emit sig_positionSignal();
                break;
            }
        }
        else if(ba.length()==11){
            //写成功
        }
    }
}

void ClientPLC::stateChanged(QAbstractSocket::SocketState socketState)
{
    switch (socketState) {
    case QAbstractSocket::SocketState::UnconnectedState:
        emit sig_constate(false);
        qDebug()<<"disconnect";
        QTimer::singleShot(2000,this,[=](){
            socket->connectToHost(hostIP,port);
        });
        break;
    case QAbstractSocket::SocketState::ConnectedState:
        qDebug()<<"connect";
        emit sig_constate(true);
        break;
    default:
        break;
    }
}

void ClientPLC::heartBeat()
{
    singleRead("D",16000);
}

QByteArray ClientPLC::buildHeader(quint16 cmd, int dataLen)
{
    QByteArray frame;
    QDataStream ds(&frame, QIODevice::WriteOnly);
    ds.setByteOrder(QDataStream::BigEndian);

    ds << quint16(0x5000);   // 副头部
    ds << netNo;      // 网络编号
    ds << PLCNo;      // PLC编号
    ds << IONo;   // IO编号
    ds << modelNo;      // 站号
    // 请求数据长度（后面所有数据）
    ds << quint16(dataLen);
    ds << quint8(overTime&0xff);   // 时钟（250ms）
    ds << quint8(overTime>>8);
    ds << quint8(cmd&0xff);
    ds << quint8(cmd>>8);
    ds << quint16(0);
    return frame;
}

quint8 ClientPLC::deviceCode(const QString &device)
{
    if (device == "D") return 0xA8;
    if (device == "M") return 0x90;
    if (device == "L") return 0x92;
    if (device == "X") return 0x9C;
    if (device == "Y") return 0x9D;
    return 0;
}

void ClientPLC::sendFrame(const QByteArray &frame)
{
    if (socket->state() != QAbstractSocket::ConnectedState) {
        emit errorOccurred("PLC not connected");
        return;
    }
    socket->write(frame);
    //qDebug()<<frame.toHex();
}
