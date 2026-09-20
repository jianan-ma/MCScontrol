#include "servertp.h"
#include <QSettings>
#include <QDebug>
ServerTP::ServerTP(QObject *parent)
    : QObject{parent}
{
    QSettings setini("config.ini",QSettings::IniFormat);
    port = setini.value("TP/port").toInt();
    dev1IP = setini.value("TP/dev1").toString();
    dev2IP = setini.value("TP/dev2").toString();
}

void ServerTP::init()
{
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &ServerTP::newConnection);
    if (!server->listen(QHostAddress::Any, port)) {
        qDebug() << "ServerTP listen failed:" << server->errorString();
    }
}

void ServerTP::Uninit()
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

void ServerTP::sendTPTestsignal(int device)
{
    if(device==1 && client1 && client1->state()==QAbstractSocket::ConnectedState){
        QString cmd = "#ST*1#";
        client1->write(cmd.toUtf8());
    }
    if(device==2 && client2 && client2->state()==QAbstractSocket::ConnectedState){
        QString cmd = "#ST*2#";
        client2->write(cmd.toUtf8());
    }
}

void ServerTP::newConnection()
{
    while (server->hasPendingConnections()) {
        QTcpSocket *socket = server->nextPendingConnection();
        if(socket->peerAddress()==QHostAddress(dev1IP)){
            if(client1){
                client1->disconnectFromHost();
                client1->deleteLater();
            }
            client1 = socket;
            connect(client1, &QTcpSocket::readyRead, this, &ServerTP::readData1);
            connect(client1, &QTcpSocket::disconnected, this, &ServerTP::clientDisconnect);
            emit sig_devConnected(1);
        }
        else if(socket->peerAddress()==QHostAddress(dev2IP)){
            if(client2){
                client2->disconnectFromHost();
                client2->deleteLater();
            }
            client2 = socket;
            connect(client2, &QTcpSocket::readyRead, this, &ServerTP::readData2);
            connect(client2, &QTcpSocket::disconnected, this, &ServerTP::clientDisconnect);
            emit sig_devConnected(2);
        }
        qDebug() << "ServerTP: new client connected," << socket->peerAddress().toString()
                 << "port:" << socket->peerPort();

    }
}

void ServerTP::clientDisconnect()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket) {
        qDebug() << "ServerTP: client disconnected," << socket->peerAddress().toString()
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

void ServerTP::readData1()
{
    QByteArray ba = client1->readAll();
    if(ba=="#ST*1*OK#"){
        //开始测试返回
    }
    else if(ba.contains("#TR*1*")){
        QString result = ba.mid(6);
        if(result=="NG#")
            emit sig_result(1,0);
        else if(result=="OK#")
            emit sig_result(1,1);
        else if(result=="TP12#")
            emit sig_result(1,2);
        else if(result=="TP12#")
            emit sig_result(1,3);
    }
}

void ServerTP::readData2()
{
    QByteArray ba = client2->readAll();
    if(ba=="#ST*2*OK#"){
        //开始测试返回
    }
    else if(ba.contains("#TR*2*")){
        QString result = ba.mid(6);
        if(result=="NG#")
            emit sig_result(2,0);
        else if(result=="OK#")
            emit sig_result(2,1);
        else if(result=="TP12#")
            emit sig_result(2,2);
        else if(result=="TP12#")
            emit sig_result(2,3);
    }
}
