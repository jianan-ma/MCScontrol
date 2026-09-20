#include "clientmanual.h"
#include <QSettings>
#include <QTimer>
ClientManual::ClientManual(QObject *parent)
    : QObject{parent}
{
    // QSettings setini("config.ini",QSettings::IniFormat);
    // hostIP = setini.value("Manual/ServerIP").toString();
    // port = setini.value("Manual/port").toInt();
}

void ClientManual::init()
{
    socket = new QTcpSocket(this);
    connect(socket,&QTcpSocket::readyRead,this,&ClientManual::readData);
    connect(socket,&QTcpSocket::stateChanged,this,&ClientManual::stateChanged);
    socket->connectToHost(hostIP,m_port);
}

void ClientManual::Uninit()
{
    socket->close();
}

void ClientManual::setIpPort(const QString &ip, const int &port)
{
    hostIP = ip;
    m_port = port;
}

void ClientManual::startCheck()
{
    QByteArray ba = QByteArray::fromHex("A5A5");
    socket->write(ba);
}

void ClientManual::readData()
{
    QByteArray ba = socket->readAll();
    if(ba.length()==3 && uchar(ba[0])==0x5a && uchar(ba[1])==0x5a){
        emit sig_checkCompelate(uchar(ba[2]));
    }
}

void ClientManual::stateChanged(QAbstractSocket::SocketState socketState)
{
    switch (socketState) {
    case QAbstractSocket::SocketState::UnconnectedState:
        emit sig_constate(false);
        QTimer::singleShot(2000,this,[=](){
            socket->connectToHost(hostIP,m_port);
        });
        break;
    case QAbstractSocket::SocketState::ConnectedState:
        emit sig_constate(true);
        break;
    default:
        break;
    }
}
