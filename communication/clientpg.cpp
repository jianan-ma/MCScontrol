#include "clientpg.h"
#include <QSettings>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDateTime>
ClientPG::ClientPG(QObject *parent)
    : QObject{parent}
{
    // QSettings setini("config.ini",QSettings::IniFormat);
    // hostIP = setini.value("PG/ServerIP").toString();
    // port = setini.value("PG/port").toInt();
}

void ClientPG::init()
{
    socket = new QTcpSocket(this);
    connect(socket,&QTcpSocket::readyRead,this,&ClientPG::readData);
    connect(socket,&QTcpSocket::stateChanged,this,&ClientPG::stateChanged);
    socket->connectToHost(hostIP,m_port);
}

void ClientPG::Uninit()
{
    socket->close();
}

void ClientPG::setIpPort(const QString &ip, const int &port)
{
    hostIP = ip;
    m_port = port;
}

void ClientPG::setParams(const QString &recipe, const QString &screenID)
{
    m_recipe = recipe;
    m_screenID = screenID;
}

void ClientPG::getStressSignal(bool needOff)
{
    if(needOff){
        needOn = true;
        PGPowerOn(false);
    }
    else{
        needOn = false;
        PGPowerOn(true);
    }
}

void ClientPG::getPGammaSignal()
{
    StartGamma();
}

void ClientPG::Initial()
{
    QJsonObject obj;
    QJsonObject obj_header;
    obj_header.insert("MessageName","Initial");
    QDateTime dt = QDateTime::currentDateTime();
    obj_header.insert("TransactionId",dt.toString("yyyyMMddhhmmsszzz"));
    obj_header.insert("UserId","AOI");
    obj_header.insert("Tag","");
    obj.insert("Header",obj_header);
    QJsonObject obj_body;
    obj_body.insert("StageID","1-1");
    obj.insert("Body",obj_body);
    QJsonObject obj_result;
    obj_result.insert("Code","1");
    obj_result.insert("Message","");
    obj.insert("Result",obj_result);
    QJsonDocument doc;
    doc.setObject(obj);
    QByteArray ba = doc.toJson(QJsonDocument::Compact);
    socket->write(ba);
}

void ClientPG::PGPowerOn(bool on)
{
    OnOrOff = on;
    QJsonObject obj;
    QJsonObject obj_header;
    obj_header.insert("MessageName","Signal");
    QDateTime dt = QDateTime::currentDateTime();
    obj_header.insert("TransactionId",dt.toString("yyyyMMddhhmmsszzz"));
    obj_header.insert("UserId","AOI");
    obj_header.insert("Tag","");
    obj.insert("Header",obj_header);
    QJsonObject obj_body;
    obj_body.insert("StageID","1-1");
    obj_body.insert("Signal",on?"On":"Off");
    obj_body.insert("CellID","Abc123");
    obj.insert("Body",obj_body);
    QJsonObject obj_result;
    obj_result.insert("Code","1");
    obj_result.insert("Message","");
    obj.insert("Result",obj_result);
    QJsonDocument doc;
    doc.setObject(obj);
    QByteArray ba = doc.toJson(QJsonDocument::Compact);
    socket->write(ba);
}

void ClientPG::StartGamma()
{
    QJsonObject obj;
    QJsonObject obj_header;
    obj_header.insert("MessageName","CellLoading");
    QDateTime dt = QDateTime::currentDateTime();
    obj_header.insert("TransactionId",dt.toString("yyyyMMddhhmmsszzz"));
    obj_header.insert("UserId","");
    obj_header.insert("Tag","");
    obj.insert("Header",obj_header);
    QJsonObject obj_body;
    obj_body.insert("ProductID","");
    obj_body.insert("RecipeID",m_recipe);
    obj_body.insert("WorkStageID_A","1-1");
    obj_body.insert("CellID_A",m_screenID);
    obj_body.insert("InspectionMode_A","1");
    obj_body.insert("WorkStageID_B","");
    obj_body.insert("CellID_B","");
    obj_body.insert("InspectionMode_B","");
    obj.insert("Body",obj_body);
    QJsonObject obj_result;
    obj_result.insert("Code","1");
    obj_result.insert("Message","");
    obj.insert("Result",obj_result);
    QJsonDocument doc;
    doc.setObject(obj);
    QByteArray ba = doc.toJson(QJsonDocument::Compact);
    socket->write(ba);
}

void ClientPG::readData()
{
    recvBuffer.append(socket->readAll());

    // 报文头标记: {"Header"  报文尾标记: "}}
    const QByteArray headTag = "{\"Header\"";
    const QByteArray tailTag = "\"}}";

    while (true) {
        int headPos = recvBuffer.indexOf(headTag);
        if (headPos == -1) {
            recvBuffer.clear();
            return;
        }

        // 从 headTag 起始位置开始找 tailTag
        int tailPos = recvBuffer.indexOf(tailTag, headPos);
        if (tailPos == -1) {
            // 还没收完，等下次 readyRead
            return;
        }

        // tailTag 之后还有数据吗？
        int msgEnd = tailPos + tailTag.length();

        // 提取完整报文（从 headPos 到 msgEnd）
        QByteArray msg = recvBuffer.mid(headPos, msgEnd - headPos);
        recvBuffer.remove(0, msgEnd);

        processMessage(msg);
    }
}

void ClientPG::processMessage(const QByteArray &msg)
{
    QJsonDocument doc = QJsonDocument::fromJson(msg);
    QJsonObject obj = doc.object();
    QJsonObject obj_header = obj.value("Header").toObject();
    QString cmd = obj_header.value("MessageName").toString();
    //如果需要日志时间
    QString s_dt = obj_header.value("TransactionId").toString();

    QJsonObject obj_result = obj.value("Result").toObject();
    QString result = obj_result.value("Code").toString();
    if(cmd=="Initial"){
        if(result=="1"){
            //初始化成功
        }
        else{
            //初始化失败
        }
    }
    else if(cmd=="Signal"){
        if(result=="1"){
            //上下电成功
            if(!OnOrOff && needOn){
                //下电成功且需要上电
                PGPowerOn(true);
                return;
            }
            if(OnOrOff){
                //上电成功
                emit sig_ToPLC_PGsuccess();
            }
        }
        else{
            //上下电失败
        }
    }
    else if(cmd=="CellLoading"){
        if(result=="1"){
            //启动gamma成功
            emit sig_ToPLC_Pgammasuccess();
        }
        else{
            //启动gamma失败
        }
    }
    else if(cmd=="InspectionResult"){
        QJsonObject obj_body = obj.value("Body").toObject();
        QString res = obj_body.value("InspectionResult_A").toString();
        if(res=="P")
            emit sig_LightUp(true);
        else
            emit sig_LightUp(false);
    }
}

void ClientPG::stateChanged(QAbstractSocket::SocketState socketState)
{
    switch (socketState) {
    case QAbstractSocket::SocketState::UnconnectedState:
        emit sig_constate(false);
        QTimer::singleShot(2000,this,[=](){
            socket->connectToHost(hostIP,m_port);
        });
        break;
    case QAbstractSocket::SocketState::ConnectedState:
        Initial();
        emit sig_constate(true);
        break;
    default:
        break;
    }
}
