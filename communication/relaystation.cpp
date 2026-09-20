#include "relaystation.h"
#include <QSettings>
RelayStation::RelayStation(QObject *parent)
    : QObject{parent}
{
    thread_PG = new QThread(this);
    clientpg = new ClientPG;
    connect(thread_PG,&QThread::started,clientpg,&ClientPG::init);
    connect(thread_PG,&QThread::finished,clientpg,&ClientPG::Uninit);
    connect(this,&RelayStation::sig_setPGIpPort,clientpg,&ClientPG::setIpPort);
    /*待连接信号槽
    connect(clientpg,&ClientPG::sig_constate,this,&RelayStation::接收PLC连接状态槽函数);
    connect(clientpg,&ClientPG::sig_ToPLC_PGsuccess,this,&RelayStation::上电成功);
    connect(clientpg,&ClientPG::sig_ToPLC_Pgammasuccess,this,&RelayStation::pgamma完成);
    connect(clientpg,&ClientPG::sig_LightUp,this,&RelayStation::点亮信号);
    connect(this,&RelayStation::设置recipe,屏id,clientpg,&ClientPG::setParams);
    connect(this,&RelayStation::PG上电下电,clientpg,&ClientPG::getStressSignal);
    connect(this,&RelayStation::开始Pgamma,clientpg,&ClientPG::getPGammaSignal);
     */
    QSettings setini("config.ini",QSettings::IniFormat);
    QString pgip = setini.value("PG1/ServerIP").toString();
    int pgport = setini.value("PG1/port").toInt();
    emit sig_setPGIpPort(pgip,pgport);
    clientpg->moveToThread(thread_PG);
    thread_PG->start();

    thread_TP = new QThread(this);
    servertp = new ServerTP;
    connect(thread_TP,&QThread::started,servertp,&ServerTP::init);
    connect(thread_TP,&QThread::finished,servertp,&ServerTP::Uninit);
    /*待连接信号槽
    connect(servertp,&ServerTP::sig_devConnected,this,&RelayStation::客户端已连接);
    connect(servertp,&ServerTP::sig_devDisConnected,this,&RelayStation::客户端断开连接);
    connect(servertp,&ServerTP::sig_result,this,&RelayStation::测试结果);
    connect(this,&RelayStation::TP测试信号,servertp,&ServerTP::sendTPTestsignal);
    */
    servertp->moveToThread(thread_TP);
    thread_TP->start();

    thread_PLC = new QThread(this);
    clientplc = new ClientPLC;
    connect(thread_PLC,&QThread::started,clientplc,&ClientPLC::init);
    connect(thread_PLC,&QThread::finished,clientplc,&ClientPLC::Uninit);
    /*待连接信号槽
    connect(clientplc,&ClientPLC::sig_constate,this,&RelayStation::接收PLC连接状态槽函数);
    connect(clientplc,&ClientPLC::sig_crimpSignal,this,&RelayStation::压接信号);可以直接连接&ClientPG::getStressSignal
    connect(clientplc,&ClientPLC::sig_PgammaSignal,this,&RelayStation::pgamma准备信号);可以直接连接&ClientPG::getPGammaSignal
    connect(clientplc,&ClientPLC::sig_APISignal,this,&RelayStation::API开始信号);
    connect(clientplc,&ClientPLC::sig_manualStationSignal,this,&RelayStation::人工站到位信号);
    connect(clientplc,&ClientPLC::sig_positionSignal,this,&RelayStation::到位信号(TP页));可以直接连接&ServerTP::sendTPTestsignal
    connect(this,&RelayStation::设置参数,clientplc,&ClientPLC::setParams);
    connect(this,&RelayStation::获取流程状态,clientplc,&ClientPLC::requestStatus);
    connect(this,&RelayStation::写入打开完成(PG),clientplc,&ClientPLC::completePG);
    connect(this,&RelayStation::写入pgamma完成,clientplc,&ClientPLC::completePgamma);
    connect(this,&RelayStation::写入API完成,clientplc,&ClientPLC::completeAPI);
    connect(this,&RelayStation::写入人工站完成,clientplc,&ClientPLC::completeManual);
    connect(this,&RelayStation::写入TP完成,clientplc,&ClientPLC::completeTP);
     */
    clientplc->moveToThread(thread_PLC);
    thread_PLC->start();

    thread_ROSE = new QThread(this);
    serverrose = new ServerRose;
    connect(thread_ROSE,&QThread::started,serverrose,&ServerRose::init);
    connect(thread_ROSE,&QThread::finished,serverrose,&ServerRose::Uninit);
    connect(clientplc,&ClientPLC::sig_sendReply,serverrose,&ServerRose::getReply);
    connect(serverrose,&ServerRose::sig_relay,clientplc,&ClientPLC::relayData);
    /*待连接信号槽
    connect(serverrose,&ServerRose::sig_devConnected,this,&&RelayStation::客户端已连接);
    connect(serverrose,&ServerRose::sig_devDisConnected,this,&&RelayStation::客户端断开连接);
     */
    serverrose->moveToThread(thread_ROSE);
    thread_ROSE->start();
}
