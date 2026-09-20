#ifndef RELAYSTATION_H
#define RELAYSTATION_H

#include <QObject>
#include <QThread>
#include "clientpg.h"
#include "servertp.h"
#include "clientplc.h"
#include "serverrose.h"
class RelayStation : public QObject
{
    Q_OBJECT
public:
    explicit RelayStation(QObject *parent = nullptr);
private:
    QThread *thread_PG;
    QThread *thread_TP;
    QThread *thread_PLC;
    QThread *thread_ROSE;
    ClientPG *clientpg;
    ServerTP *servertp;
    ClientPLC *clientplc;
    ServerRose *serverrose;
signals:
    void sig_setPGIpPort(const QString&,const int&);
    void sig_setManualIpPort(const QString&,const int&);
};

#endif // RELAYSTATION_H
