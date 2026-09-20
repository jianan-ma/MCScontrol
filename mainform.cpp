#include "mainform.h"
#include "mainform.h"
#include <QButtonGroup>
#include <QToolButton>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSettings>
#include "mainconfig.h"
#include "manualdebug.h"
#include "logquery.h"
#include "statistics.h"

MainForm::MainForm(QWidget *parent)
    : QWidget{parent}
{
    setWindowFlags(Qt::FramelessWindowHint);
    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    mainlayout->setContentsMargins(0, 0, 0, 0);
    mainlayout->setSpacing(0);

    // 创建导航栏容器
    QWidget *navBar = new QWidget(this);
    navBar->setObjectName("navBar");
    navBar->setFixedHeight(45);
    navBar->setStyleSheet(
        "QWidget#navBar {"
        "    background-color: #1e2a3a;"
        "}"
        "QToolButton {"
        "    color: #d0d8e0;"
        "    background-color: transparent;"
        "    border: none;"
        "    border-radius: 4px;"
        "    padding: 6px 18px;"
        "    font-size: 13px;"
        "    font-family: 'Microsoft YaHei', sans-serif;"
        "}"
        "QToolButton:hover {"
        "    color: #ffffff;"
        "    background-color: rgba(255, 255, 255, 15);"
        "}"
        "QToolButton:checked {"
        "    color: #1a1a2e;"
        "    background-color: #e8ecf1;"
        "}"
        "QPushButton#winBtn {"
        "    color: #d0d8e0;"
        "    background-color: transparent;"
        "    border: none;"
        "    border-radius: 3px;"
        "    font-size: 14px;"
        "    min-width: 36px;"
        "    min-height: 28px;"
        "}"
        "QPushButton#winBtn:hover {"
        "    background-color: rgba(255, 255, 255, 20);"
        "}"
        "QPushButton#btnClose:hover {"
        "    background-color: #e81123;"
        "    color: #ffffff;"
        "}"
        );

    // 1. 初始化按钮组（实现单选互斥）
    QButtonGroup *navGroup = new QButtonGroup(this);
    navGroup->setExclusive(true);
    // 2. 创建导航按钮（推荐用 QToolButton，对 Icon 支持更好）
    QToolButton *btnHome = new QToolButton(navBar);
    btnHome->setCheckable(true);
    btnHome->setText(" 主界面");
    btnHome->setIcon(QIcon(":/icon/res/main_w.png"));
    btnHome->setIconSize(QSize(24, 24));
    btnHome->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnHome->setCursor(Qt::PointingHandCursor);

    QToolButton *btnFormula = new QToolButton(navBar);
    btnFormula->setCheckable(true);
    btnFormula->setText(" 配方");
    btnFormula->setIcon(QIcon(":/icon/res/formula_w.png"));
    btnFormula->setIconSize(QSize(24, 24));
    btnFormula->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnFormula->setCursor(Qt::PointingHandCursor);

    QToolButton *btnConfig = new QToolButton(navBar);
    btnConfig->setCheckable(true);
    btnConfig->setText(" 配置");
    btnConfig->setIcon(QIcon(":/icon/res/config_w.png"));
    btnConfig->setIconSize(QSize(24, 24));
    btnConfig->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnConfig->setCursor(Qt::PointingHandCursor);

    QToolButton *btnDebug = new QToolButton(navBar);
    btnDebug->setCheckable(true);
    btnDebug->setText(" 调试");
    btnDebug->setIcon(QIcon(":/icon/res/debug_w.png"));
    btnDebug->setIconSize(QSize(24, 24));
    btnDebug->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnDebug->setCursor(Qt::PointingHandCursor);

    QToolButton *btnStats = new QToolButton(navBar);
    btnStats->setCheckable(true);
    btnStats->setText(" 统计");
    btnStats->setIcon(QIcon(":/icon/res/statistics_w.png"));
    btnStats->setIconSize(QSize(24, 24));
    btnStats->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnStats->setCursor(Qt::PointingHandCursor);

    QToolButton *btnLog = new QToolButton(navBar);
    btnLog->setCheckable(true);
    btnLog->setChecked(true);
    btnLog->setText(" 日志");
    btnLog->setIcon(QIcon(":/icon/res/log.png"));
    btnLog->setIconSize(QSize(24, 24));
    btnLog->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btnLog->setCursor(Qt::PointingHandCursor);

    // 将按钮加入组
    navGroup->addButton(btnHome, 0);
    navGroup->addButton(btnFormula, 1);
    navGroup->addButton(btnConfig, 2);
    navGroup->addButton(btnDebug,3);
    navGroup->addButton(btnStats, 4);
    navGroup->addButton(btnLog, 5);

    // 3. 创建右侧窗口控制按钮
    QPushButton *btnMin = new QPushButton(navBar);
    btnMin->setObjectName("winBtn");
    btnMin->setCursor(Qt::PointingHandCursor);
    btnMin->setStyleSheet("border-image: url(:/icon/res/min.png);");
    btnMin->setFixedSize(32,32);

    QPushButton *btnMax = new QPushButton(navBar);
    btnMax->setObjectName("winBtn");
    btnMax->setCursor(Qt::PointingHandCursor);
    btnMax->setStyleSheet("border-image: url(:/icon/res/maxd.png);");
    btnMax->setFixedSize(32,32);

    QPushButton *btnClose = new QPushButton(navBar);
    btnClose->setObjectName("btnClose");
    btnClose->setCursor(Qt::PointingHandCursor);
    btnClose->setStyleSheet("border-image: url(:/icon/res/close.png);");
    btnClose->setFixedSize(32,32);

    // 4. 布局组装
    QHBoxLayout *navLayout = new QHBoxLayout(navBar);
    navLayout->setContentsMargins(8, 4, 4, 4);
    navLayout->setSpacing(2);
    navLayout->addWidget(btnHome);
    navLayout->addWidget(btnFormula);
    navLayout->addWidget(btnConfig);
    navLayout->addWidget(btnDebug);
    navLayout->addWidget(btnStats);
    navLayout->addWidget(btnLog);
    navLayout->addStretch();
    navLayout->addWidget(btnMin);
    navLayout->addWidget(btnMax);
    navLayout->addWidget(btnClose);
    mainlayout->addWidget(navBar);

    stackedwidget = new QStackedWidget;
    mainwidget = new QWidget;
    QVBoxLayout *vlayout = new QVBoxLayout;

    gridlayout = new QGridLayout;
    vlayout->addLayout(gridlayout,3);
    mainwidget->setLayout(vlayout);
    stackedwidget->addWidget(mainwidget);
    stackedwidget->setCurrentIndex(0);
    mainlayout->addWidget(stackedwidget, 1);

    realtimeLog = new RealTimeLog;
    vlayout->addWidget(realtimeLog,1);

    // 5. 绑定切换逻辑（核心：动态切换 Icon + 切换 StackedWidget 页面）
    connect(navGroup, QOverload<int, bool>::of(&QButtonGroup::buttonToggled), this, [=](int id, bool checked) {
        if (checked) {
            switch (id) {
            case 0: btnHome->setIcon(QIcon(":/icon/res/main.png")); break;
            case 1: btnFormula->setIcon(QIcon(":/icon/res/formula.png")); break;
            case 2: btnConfig->setIcon(QIcon(":/icon/res/config.png")); break;
            case 3: btnDebug->setIcon(QIcon(":/icon/res/debug.png")); break;
            case 4: btnStats->setIcon(QIcon(":/icon/res/statistics.png")); break;
            case 5: btnLog->setIcon(QIcon(":/icon/res/log.png")); break;
            }
        } else {
            switch (id) {
            case 0: btnHome->setIcon(QIcon(":/icon/res/main_w.png")); break;
            case 1: btnFormula->setIcon(QIcon(":/icon/res/formula_w.png")); break;
            case 2: btnConfig->setIcon(QIcon(":/icon/res/config_w.png")); break;
            case 3: btnDebug->setIcon(QIcon(":/icon/res/debug_w.png")); break;
            case 4: btnStats->setIcon(QIcon(":/icon/res/statistics_w.png")); break;
            case 5: btnLog->setIcon(QIcon(":/icon/res/log_w.png")); break;
            }
        }
        stackedwidget->setCurrentIndex(id);
    });
    btnHome->toggle();
    // 6. 窗口控制按钮信号绑定
    connect(btnMin, &QPushButton::clicked, this, &QWidget::showMinimized);
    connect(btnClose, &QPushButton::clicked, this, &QWidget::close);
    connect(btnMax, &QPushButton::clicked, this, [this,btnMax](){
        if(windowState()==2 || windowState()==6){
            showNormal();
            btnMax->setStyleSheet("border-image: url(:/icon/res/maxd.png);");
        }
        else{
            showMaximized();
            btnMax->setStyleSheet("border-image: url(:/icon/res/max.png);");
        }
    });

    QWidget *emptywid = new QWidget;
    stackedwidget->addWidget(emptywid);
    MainConfig *mainconfig = new MainConfig;
    stackedwidget->addWidget(mainconfig);
    ManualDebug *manualdebug = new ManualDebug;
    stackedwidget->addWidget(manualdebug);
    Statistics *statistics = new Statistics;
    stackedwidget->addWidget(statistics);
    LogQuery *logquery = new LogQuery;
    stackedwidget->addWidget(logquery);

    //示例代码：添加工位
    // WorkStation *station1 = new WorkStation(this);
    // station1->setStationNumber("A1");
    // addWorkStation(station1);
    // WorkStation *station2 = new WorkStation(this);
    // station2->setStationNumber("A2");
    // addWorkStation(station2);
    // WorkStation *station3 = new WorkStation(this);
    // station3->setStationNumber("A3");
    // addWorkStation(station3);
    // WorkStation *station4 = new WorkStation(this);
    // station4->setStationNumber("A4");
    // addWorkStation(station4);
    // WorkStation *station5 = new WorkStation(this);
    // station5->setStationNumber("A5");
    // addWorkStation(station5);
    StationWidget *station1 = new StationWidget(this);
    station1->setLabel1("转盘1");
    station1->setTopLabels("测试文本","测试文本","测试文本","测试文本");
    station1->setLeftLabels("测试文本","测试文本","测试文本","测试文本");
    station1->setBottomLabels("测试文本","测试文本","测试文本","测试文本");
    station1->setRightLabels("测试文本","测试文本","测试文本","测试文本");
    StationWidget *station2 = new StationWidget(this);
    station2->setLabel1("转盘2");
    station2->setTopLabels("测试文本","测试文本","测试文本","测试文本");
    station2->setLeftLabels("测试文本","测试文本","测试文本","测试文本");
    station2->setBottomLabels("测试文本","测试文本","测试文本","测试文本");
    station2->setRightLabels("测试文本","测试文本","测试文本","测试文本");
    addWorkStation(station1);
    addWorkStation(station2);

    //示例代码：添加日志
    realtimeLog->addLog(RealTimeLog::Error,"16:41:23.345","A1","工位服务器断开连接");
    realtimeLog->addLog(RealTimeLog::Error,"16:41:23.345","A2","工位服务器断开连接");
    realtimeLog->addLog(RealTimeLog::Error,"16:41:23.345","A3","工位服务器断开连接");
    realtimeLog->addLog(RealTimeLog::Warning,"16:41:23.345","A1","工位服务器断开连接");
    realtimeLog->addLog(RealTimeLog::Warning,"16:41:23.345","A2","工位服务器断开连接");
    realtimeLog->addLog(RealTimeLog::Warning,"16:41:23.345","A3","工位服务器断开连接");
    realtimeLog->addLog(RealTimeLog::Warning,"16:41:23.345","A4","工位服务器断开连接");
    realtimeLog->addLog(RealTimeLog::Info,"16:41:23.345","A1","工位服务器断开连接");
    realtimeLog->addLog(RealTimeLog::Info,"16:41:23.345","A2","工位服务器断开连接");
    realtimeLog->addLog(RealTimeLog::Info,"16:41:23.345","A3","工位服务器断开连接");
    realtimeLog->addLog(RealTimeLog::Info,"16:41:23.345","A4","工位服务器断开连接");

    //临时添加PG
    thread_PG = new QThread(this);
    clientpg = new ClientPG;
    connect(thread_PG,&QThread::started,clientpg,&ClientPG::init);
    connect(thread_PG,&QThread::finished,clientpg,&ClientPG::Uninit);
    connect(manualdebug,&ManualDebug::sig_PgInit,clientpg,&ClientPG::getStressSignal);
    connect(manualdebug,&ManualDebug::sig_PgPgamma,clientpg,&ClientPG::getPGammaSignal);
    connect(clientpg,&ClientPG::sig_constate,manualdebug,&ManualDebug::PgConnect);
    connect(clientpg,&ClientPG::sig_ToPLC_PGsuccess,manualdebug,&ManualDebug::PgSuccess);
    connect(clientpg,&ClientPG::sig_ToPLC_Pgammasuccess,manualdebug,&ManualDebug::PgammaSuccess);
    connect(clientpg,&ClientPG::sig_LightUp,manualdebug,&ManualDebug::LightUp);
    QSettings setini("config.ini",QSettings::IniFormat);
    QString pgip = setini.value("PG1/ServerIP").toString();
    int pgport = setini.value("PG1/port").toInt();
    clientpg->setIpPort(pgip,pgport);
    clientpg->moveToThread(thread_PG);
    thread_PG->start();
}

MainForm::~MainForm()
{
    thread_PG->quit();
    thread_PG->wait();
    thread_PG->deleteLater();
}

void MainForm::addWorkStation(WorkStation *station)
{
    gridlayout->addWidget(station,stationNums/3,stationNums%3);
    stationNums++;
}

void MainForm::addWorkStation(StationWidget *station)
{
    gridlayout->addWidget(station,stationNums/2,stationNums%2);
    stationNums++;
}

void MainForm::mousePressEvent(QMouseEvent *event)
{
    if(event->y()<45){
        isMoving = true;
        startPoint = event->pos();
    }
}

void MainForm::mouseMoveEvent(QMouseEvent *event)
{
    if(isMoving){
        this->move(this->x()+event->x()-startPoint.x(),this->y()+event->y()-startPoint.y());
    }
}

void MainForm::mouseReleaseEvent(QMouseEvent *event)
{
    isMoving = false;
    startPoint = event->pos();
}
