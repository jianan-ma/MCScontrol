#include "serverrose.h"
#include <QSettings>
#include <QDebug>
ServerRose::ServerRose(QObject *parent)
    : QObject{parent}
{
    QSettings setini("config.ini",QSettings::IniFormat);
    port = setini.value("Rose/port").toInt();
    dev1IP = setini.value("TP/dev1").toString();
    dev2IP = setini.value("TP/dev2").toString();
}

void ServerRose::init()
{
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &ServerRose::newConnection);
    if (!server->listen(QHostAddress::Any, port)) {
        qDebug() << "ServerRose listen failed:" << server->errorString();
    }
}

void ServerRose::Uninit()
{
    server->close();
    if(client1){
        client1->disconnectFromHost();
        client1->deleteLater();
    }
    if(client2){
        client2->disconnectFromHost();
        client2->deleteLater();
    }
}

void ServerRose::getReply(int id,QByteArray ba)
{
    if(id==1)
        client1->write(ba);
    else if(id==2)
        client2->write(ba);
}

void ServerRose::newConnection()
{
    while (server->hasPendingConnections()) {
        QTcpSocket *socket = server->nextPendingConnection();
        if(socket->peerAddress()==QHostAddress(dev1IP)){
            if(client1){
                client1->disconnectFromHost();
                client1->deleteLater();
            }
            client1 = socket;
            connect(client1, &QTcpSocket::readyRead, this, &ServerRose::readData1);
            connect(client1, &QTcpSocket::disconnected, this, &ServerRose::clientDisconnect);
            emit sig_devConnected(1);
        }
        else if(socket->peerAddress()==QHostAddress(dev2IP)){
            if(client2){
                client2->disconnectFromHost();
                client2->deleteLater();
            }
            client2 = socket;
            connect(client2, &QTcpSocket::readyRead, this, &ServerRose::readData2);
            connect(client2, &QTcpSocket::disconnected, this, &ServerRose::clientDisconnect);
            emit sig_devConnected(2);
        }
        qDebug() << "ServerRose: new client connected," << socket->peerAddress().toString()
                 << "port:" << socket->peerPort();

    }
}

void ServerRose::clientDisconnect()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket) {
        qDebug() << "ServerRose: client disconnected," << socket->peerAddress().toString()
        << "port:" << socket->peerPort();
        if(socket->peerAddress()==QHostAddress(dev1IP)){
            emit sig_devDisConnected(1);
        }
        else if(socket->peerAddress()==QHostAddress(dev2IP)){
            emit sig_devDisConnected(2);
        }
        socket->deleteLater();
    }
}

void ServerRose::readData1()
{
    QByteArray ba = client1->readAll();
    emit sig_relay(1,ba);
}

void ServerRose::readData2()
{
    QByteArray ba = client2->readAll();
    emit sig_relay(2,ba);
}

