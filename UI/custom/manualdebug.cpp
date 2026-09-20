#include "manualdebug.h"
#include <QGridLayout>
#include <QScrollArea>
#include <QPropertyAnimation>
#include <QFormLayout>
#include <QRadioButton>
#include <QButtonGroup>
ManualDebug::ManualDebug(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void ManualDebug::setupUI()
{
    setStyleSheet(
        "ManualDebug {"
        "    background-color: #f5f7fa;"
        "}"
        "QGroupBox {"
        "    border: 1px solid #d0d7de;"
        "    border-radius: 6px;"
        "    margin-top: 12px;"
        "    font-size: 13px;"
        "    font-weight: bold;"
        "    color: #4a5568;"
        "    padding-top: 8px;"
        "}"
        "QGroupBox::title {"
        "    subcontrol-origin: margin;"
        "    subcontrol-position: top left;"
        "    left: 12px;"
        "    padding: 0 6px;"
        "}"
        "QLabel {"
        "    color: #1a1a2e;"
        "    font-size: 13px;"
        "}"
        "QPushButton {"
        "    background-color: #4a90d9;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 4px;"
        "    padding: 6px 14px;"
        "    font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #357abd;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #2c6aa0;"
        "}"
        "QPushButton#toggleBtn {"
        "    background-color: #e0e6ed;"
        "    color: #4a5568;"
        "    border: 1px solid #cfd5de;"
        "    border-radius: 12px;"
        "    padding: 4px 20px;"
        "    font-size: 12px;"
        "}"
        "QPushButton#toggleBtn:checked {"
        "    background-color: #2ecc71;"
        "    border: 1px solid #27ae60;"
        "    color: white;"
        "}"
        "QPushButton#ghostBtn {"
        "    background-color: transparent;"
        "    color: #4a5568;"
        "    border: 1px solid #cfd8e3;"
        "    border-radius: 4px;"
        "    padding: 4px 10px;"
        "    font-size: 12px;"
        "}"
        "QPushButton#ghostBtn:hover {"
        "    background-color: #e8edf3;"
        "}"
        "QLineEdit {"
        "    border: 1px solid #cfd8e3;"
        "    border-radius: 4px;"
        "    padding: 4px 8px;"
        "    font-size: 13px;"
        "    color: #1a1a2e;"
        "}"
        "QLineEdit:focus {"
        "    border: 1px solid #4a90d9;"
        "}"
        "QTreeWidget {"
        "    border: none;"
        "    background: transparent;"
        "    font-size: 12px;"
        "    color: #4a5568;"
        "    outline: none;"
        "}"
        "QTreeWidget::item {"
        "    padding: 4px 2px;"
        "    border-bottom: none;"
        "}"
        "QTreeWidget::item:selected {"
        "    background-color: #e3f2fd;"
        "    color: #1a1a2e;"
        "}"
        "QCheckBox {"
        "    color: #4a5568;"
        "    font-size: 12px;"
        "    spacing: 4px;"
        "}"
        "QCheckBox::indicator {"
        "    width: 14px;"
        "    height: 14px;"
        "    border: 1px solid #cfd8e3;"
        "    border-radius: 3px;"
        "    background-color: white;"
        "}"
        "QCheckBox::indicator:checked {"
        "    background-color: #2ecc71;"
        "    border: 1px solid #27ae60;"
        "}"
        "QRadioButton {"
        "    color: #4a5568;"
        "    font-size: 12px;"
        "    spacing: 4px;"
        "}"
        "QRadioButton::indicator {"
        "    width: 14px;"
        "    height: 14px;"
        "    border: 1px solid #cfd8e3;"
        "    border-radius: 7px;"
        "    background-color: white;"
        "}"
        "QRadioButton::indicator:checked {"
        "    background-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,"
        "        stop:0 white, stop:0.5 white, stop:0.52 dark, stop:1 dark);"
        "    border: 2px solid #4a90d9;"
        "}"
        "QRadioButton#rdoNormal {"
        "    color: #2ecc71;"
        "    font-weight: bold;"
        "}"
        "QRadioButton#rdoNormal::indicator:checked {"
        "    border: 2px solid #2ecc71;"
        "    background-color: #2ecc71;"
        "}"
        "QRadioButton#rdoWarning {"
        "    color: #f5a623;"
        "    font-weight: bold;"
        "}"
        "QRadioButton#rdoWarning::indicator:checked {"
        "    border: 2px solid #f5a623;"
        "    background-color: #f5a623;"
        "}"
        "QRadioButton#rdoAlarm {"
        "    color: #e74c3c;"
        "    font-weight: bold;"
        "}"
        "QRadioButton#rdoAlarm::indicator:checked {"
        "    border: 2px solid #e74c3c;"
        "    background-color: #e74c3c;"
        "}"
        "QTabWidget::pane {"
        "    border: 1px solid #d0d7de;"
        "    border-radius: 6px;"
        "    background: white;"
        "    top: -1px;"
        "}"
        "QTabBar::tab {"
        "    background: #e8edf3;"
        "    color: #4a5568;"
        "    padding: 8px 28px;"
        "    min-width: 80px;"
        "    border-top-left-radius: 6px;"
        "    border-top-right-radius: 6px;"
        "    margin-right: 2px;"
        "}"
        "QTabBar::tab:selected {"
        "    background: white;"
        "    color: #1a1a2e;"
        "    font-weight: bold;"
        "    padding-left: 32px;"
        "    padding-right: 32px;"
        "}"
        "QTabBar::tab:hover:!selected {"
        "    background: #dde4ec;"
        "}"
        "QScrollArea {"
        "    border: none;"
        "    background: transparent;"
        "}"
        "QScrollBar:vertical {"
        "    background-color: #f0f4f8;"
        "    width: 8px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background-color: #cfd8e3;"
        "    border-radius: 4px;"
        "    min-height: 30px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "    background-color: #b0bac5;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    height: 0px;"
        "}"
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 改用 QHBoxLayout：侧边栏固定宽度、不需要拖拽时，比 QSplitter 更紧凑（无 handle 间隙）
    QHBoxLayout *bodyLayout = new QHBoxLayout();
    bodyLayout->setContentsMargins(0, 0, 0, 0);
    bodyLayout->setSpacing(0);

    // 左侧边栏
    QWidget *sidebar = createSidebar();
    sidebar->setFixedWidth(220);
    bodyLayout->addWidget(sidebar);

    // 右侧主区域
    m_tabWidget = new QTabWidget(this);

    // 仅"PLC信号"tab
    QWidget *plcSignalTab = createPlcSignalTab();
    m_tabWidget->addTab(plcSignalTab, "PLC信号");
    QWidget *PgTab = createPgTab();
    m_tabWidget->addTab(PgTab, "PG");
    m_tabWidget->addTab(new QWidget(), "电机");
    m_tabWidget->addTab(new QWidget(), "统计");
    m_tabWidget->addTab(new QWidget(), "调机控制");

    bodyLayout->addWidget(m_tabWidget, 1);

    m_tabWidget->tabBar()->setUsesScrollButtons(true);

    mainLayout->addLayout(bodyLayout);
}

QWidget *ManualDebug::createSidebar()
{
    QWidget *sidebar = new QWidget(this);
    sidebar->setStyleSheet(
        "QWidget {"
        "    background-color: #ffffff;"
        "    border-right: 1px solid #e0e6ed;"
        "}"
    );

    QVBoxLayout *layout = new QVBoxLayout(sidebar);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(12);

    // === 调试模式 ===
    QHBoxLayout *modeLayout = new QHBoxLayout();
    QLabel *modeLabel = new QLabel("调试模式", sidebar);
    m_btnDebugMode = new QPushButton(sidebar);
    m_btnDebugMode->setObjectName("toggleBtn");
    m_btnDebugMode->setCheckable(true);
    m_btnDebugMode->setText("OFF");
    m_btnDebugMode->setChecked(false);
    connect(m_btnDebugMode, &QPushButton::toggled, this, [this](bool checked) {
        m_btnDebugMode->setText(checked ? "ON" : "OFF");
    });
    modeLayout->addWidget(modeLabel);
    modeLayout->addStretch();
    modeLayout->addWidget(m_btnDebugMode);
    layout->addLayout(modeLayout);

    // === 系统状态 ===
    QGroupBox *statusGroup = new QGroupBox("系统状态", sidebar);
    QVBoxLayout *statusVLayout = new QVBoxLayout(statusGroup);
    statusVLayout->setSpacing(8);
    statusVLayout->setContentsMargins(12, 24, 12, 12);

    // 状态单选按钮（垂直排列，清晰易点击）
    QHBoxLayout *radioRow1 = new QHBoxLayout();
    radioRow1->setSpacing(10);

    m_rdoNormal = new QRadioButton("正常", statusGroup);
    m_rdoNormal->setObjectName("rdoNormal");
    m_rdoNormal->setFixedHeight(30);
    m_rdoWarning = new QRadioButton("预警", statusGroup);
    m_rdoWarning->setObjectName("rdoWarning");
    m_rdoWarning->setFixedHeight(30);
    m_rdoAlarm = new QRadioButton("报警", statusGroup);
    m_rdoAlarm->setObjectName("rdoAlarm");
    m_rdoAlarm->setFixedHeight(30);
    m_statusGroup = new QButtonGroup(statusGroup);
    m_statusGroup->addButton(m_rdoNormal, StatusNormal);
    m_statusGroup->addButton(m_rdoWarning, StatusWarning);
    m_statusGroup->addButton(m_rdoAlarm, StatusAlarm);
    m_rdoNormal->setChecked(true);

    radioRow1->addWidget(m_rdoNormal);
    radioRow1->addWidget(m_rdoWarning);
    radioRow1->addWidget(m_rdoAlarm);

    // 报警按钮 - 与状态按钮上下对齐，横向居中
    QHBoxLayout *btnRow = new QHBoxLayout();
    btnRow->setSpacing(8);
    btnRow->setContentsMargins(0, 4, 0, 0);

    m_btnAlarmList = new QPushButton("报警列表", statusGroup);
    m_btnAlarmList->setObjectName("ghostBtn");
    m_btnAlarmList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_btnClearAlarm = new QPushButton("清除报警", statusGroup);
    m_btnClearAlarm->setObjectName("ghostBtn");
    m_btnClearAlarm->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    btnRow->addWidget(m_btnAlarmList);
    btnRow->addWidget(m_btnClearAlarm);

    statusVLayout->addLayout(radioRow1);
    statusVLayout->addLayout(btnRow);

    layout->addWidget(statusGroup);

    // === 用户信息 ===
    QGroupBox *userGroup = new QGroupBox("用户信息", sidebar);
    QFormLayout *userLayout = new QFormLayout(userGroup);
    userLayout->setLabelAlignment(Qt::AlignRight);
    userLayout->setSpacing(8);
    userLayout->setContentsMargins(8, 20, 8, 8);

    m_edtAccount = new QLineEdit(userGroup);
    m_edtAccount->setPlaceholderText("请输入账号");
    m_edtAccount->setFixedWidth(140);

    m_btnLogin = new QPushButton("登录", userGroup);
    m_btnLogin->setFixedWidth(80);

    userLayout->addRow("账号:", m_edtAccount);
    QHBoxLayout *loginRow = new QHBoxLayout();
    loginRow->addStretch();
    loginRow->addWidget(m_btnLogin);
    userLayout->addRow(loginRow);

    layout->addWidget(userGroup);

    // === 工位列表 ===
    QGroupBox *stationGroup = new QGroupBox("工位", sidebar);
    QVBoxLayout *stationLayout = new QVBoxLayout(stationGroup);
    stationLayout->setContentsMargins(4, 20, 4, 4);

    m_treeStations = new QTreeWidget(stationGroup);
    m_treeStations->setHeaderHidden(true);
    m_treeStations->setStyleSheet(
        "QTreeWidget {"
        "    border: none;"
        "    background: transparent;"
        "    font-size: 12px;"
        "    color: #4a5568;"
        "    outline: none;"
        "}"
        "QTreeWidget::item {"
        "    padding: 4px 2px;"
        "}"
        "QTreeWidget::item:selected {"
        "    background-color: #e3f2fd;"
        "    color: #1a1a2e;"
        "}"
    );

    // 工位1
    QTreeWidgetItem *station1 = new QTreeWidgetItem(m_treeStations, {"工站1"});
    station1->setData(0, Qt::UserRole, "fupan1");
    QTreeWidgetItem *fupan1 = new QTreeWidgetItem(station1, {"● fupan1"});
    fupan1->setForeground(0, QColor("#2ecc71"));

    // 工位3
    QTreeWidgetItem *station3 = new QTreeWidgetItem(m_treeStations, {"工站3"});
    QTreeWidgetItem *aoi1 = new QTreeWidgetItem(station3, {"● AOI1"});
    aoi1->setForeground(0, QColor("#2ecc71"));
    QTreeWidgetItem *aoi2 = new QTreeWidgetItem(station3, {"● AOI2"});
    aoi2->setForeground(0, QColor("#2ecc71"));
    QTreeWidgetItem *aoi3 = new QTreeWidgetItem(station3, {"● AOI3"});
    aoi3->setForeground(0, QColor("#2ecc71"));
    QTreeWidgetItem *aoi4 = new QTreeWidgetItem(station3, {"● AOI4"});
    aoi4->setForeground(0, QColor("#2ecc71"));

    m_treeStations->expandAll();
    stationLayout->addWidget(m_treeStations);
    layout->addWidget(stationGroup, 1);

    // === 客户端 ===
    QGroupBox *clientGroup = new QGroupBox("客户端", sidebar);
    QGridLayout *clientLayout = new QGridLayout(clientGroup);
    clientLayout->setContentsMargins(8, 20, 8, 8);
    clientLayout->setHorizontalSpacing(12);
    clientLayout->setVerticalSpacing(6);

    auto addClient = [&](const QString &name, int row, int col) {
        QLabel *dot = new QLabel("●", clientGroup);
        dot->setStyleSheet("color: #2ecc71; font-size: 12px;");
        QLabel *lbl = new QLabel(name, clientGroup);
        lbl->setStyleSheet("font-size: 12px; color: #4a5568;");
        clientLayout->addWidget(dot, row, col * 2);
        clientLayout->addWidget(lbl, row, col * 2 + 1);
    };

    addClient("PLC", 0, 0);
    addClient("PG", 0, 1);
    addClient("TP", 1, 0);
    addClient("Tibco", 1, 1);

    layout->addWidget(clientGroup);

    return sidebar;
}

QWidget *ManualDebug::createPlcSignalTab()
{
    QWidget *tab = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(tab);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(8);

    QScrollArea *scrollArea = new QScrollArea(tab);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet(
        "QScrollArea {"
        "    border: none;"
        "    background: transparent;"
        "}"
    );

    QWidget *scrollContent = new QWidget(scrollArea);
    QHBoxLayout *contentLayout = new QHBoxLayout(scrollContent);
    contentLayout->setContentsMargins(4, 4, 4, 4);
    contentLayout->setSpacing(12);

    // === 上料 ===
    QGroupBox *loadGroup = new QGroupBox("上料", scrollContent);
    QVBoxLayout *loadLayout = new QVBoxLayout(loadGroup);
    loadLayout->setContentsMargins(12, 22, 12, 12);
    loadLayout->setSpacing(8);

    // 上料内：PLC信号 + EDC信号 并排
    QHBoxLayout *loadInner = new QHBoxLayout();
    loadInner->setSpacing(8);

    QStringList loadPlc = {
        "[D30000.0]心跳",
        "[D30001.0]面板到位",
        "[D30001.1]面板1请求开电",
        "[D30001.2]面板2请求开电",
        "[D30001.3]面板3请求开电",
        "[D30001.5]面板4请求开电",
        "[D30001.7]面板4请求开电",
        "[D30001.2]面板请求关电",
        "[D30001.4]面板2请求关电",
        "[D30001.6]面板3请求关电",
        "[D30001.8]面板4请求关电",
        "[D30000.0]手启动"
    };

    QStringList loadEdc = {
        "[D30050.0]心跳",
        "[D30050.1]面板1到位回复",
        "[D30050.1]面板1开电完成",
        "[D30050.3]面板2开电完成",
        "[D30051.1]面板3开电完成",
        "[D30051.7]面板4开电完成",
        "[D30050.1]面板1关电完成",
        "[D30050.2]面板1完成",
        "[D30050.4]面板2关电完成",
        "[D30050.6]面板3关电完成",
        "[D30050.8]面板4关电完成",
        "[D30052.0]面板1开电OK"
    };

    loadInner->addWidget(createSignalGroup("PLC信号", loadPlc, 1));
    loadInner->addWidget(createSignalGroup("EDC信号", loadEdc, 1));

    loadLayout->addLayout(loadInner);
    contentLayout->addWidget(loadGroup);

    // === GAMMA ===
    QGroupBox *gammaGroup = new QGroupBox("GAMMA", scrollContent);
    QVBoxLayout *gammaLayout = new QVBoxLayout(gammaGroup);
    gammaLayout->setContentsMargins(12, 22, 12, 12);
    gammaLayout->setSpacing(8);

    QHBoxLayout *gammaInner = new QHBoxLayout();
    gammaInner->setSpacing(8);

    QStringList gammaPlc = {
        "[D30002.0]面板到位",
        "[D30002.1]请求GAMMA检测"
    };

    QStringList gammaLcs = {
        "[D30050.2]面板到位回复",
        "[D30050.1]GAMMA检测完成"
    };

    gammaInner->addWidget(createSignalGroup("PLC信号", gammaPlc, 1));
    gammaInner->addWidget(createSignalGroup("LCS信号", gammaLcs, 1));

    gammaLayout->addLayout(gammaInner);
    contentLayout->addWidget(gammaGroup);

    // === AOI ===
    QGroupBox *aoiGroup = new QGroupBox("AOI", scrollContent);
    QVBoxLayout *aoiLayout = new QVBoxLayout(aoiGroup);
    aoiLayout->setContentsMargins(12, 22, 12, 12);
    aoiLayout->setSpacing(8);

    QHBoxLayout *aoiInner = new QHBoxLayout();
    aoiInner->setSpacing(8);

    QStringList aoiPlc = {
        "[D30003.0]面板到位",
        "[D30003.1]请求AOI检测",
        "[D30003.6]响应开始自动对焦",
        "[D30003.7]响应结束自动对焦",
        "[D30003.8]电机移动完成",
        "[D30003.9]PLC保存机位位置"
    };

    QStringList aoiLcs = {
        "[D30050.3]面板到位回复",
        "[D30050.3]AOI检测完成",
        "[D30050.6]响应开始自动对焦",
        "[D30050.7]结束自动对焦",
        "[D30050.8]请求电机移动",
        "[D30050.3]保存机位位置"
    };

    aoiInner->addWidget(createSignalGroup("PLC信号", aoiPlc, 1));
    aoiInner->addWidget(createSignalGroup("LCS信号", aoiLcs, 1));

    aoiLayout->addLayout(aoiInner);
    contentLayout->addWidget(aoiGroup);

    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);

    return tab;
}

QWidget *ManualDebug::createPgTab()
{
    QWidget *tab = new QWidget();
    QPushButton *btn_init = new QPushButton("PG初始化上电",tab);
    chk = new QCheckBox("需下电",tab);
    textedit = new QTextEdit(tab);
    QPushButton *btn_pgama = new QPushButton("开始Pgamma",tab);
    btn_init->move(120,20);
    chk->move(20,20);
    textedit->move(20,180);
    btn_pgama->move(20,100);
    connect(btn_init,&QPushButton::clicked,this,[this](){
        emit sig_PgInit(chk->isChecked());
    });
    connect(btn_pgama,&QPushButton::clicked,this,&ManualDebug::sig_PgPgamma);
    return tab;
}

QGroupBox *ManualDebug::createSignalGroup(const QString &title,
                                           const QStringList &psignals,
                                           int columns)
{
    QGroupBox *group = new QGroupBox(title);
    QVBoxLayout *outerLayout = new QVBoxLayout(group);
    outerLayout->setContentsMargins(8, 20, 8, 8);
    outerLayout->setSpacing(4);

    QGridLayout *grid = new QGridLayout();
    grid->setHorizontalSpacing(12);
    grid->setVerticalSpacing(4);

    for (int i = 0; i < psignals.size(); ++i) {
        QCheckBox *cb = new QCheckBox(psignals[i], group);
        cb->setStyleSheet(
            "QCheckBox {"
            "    color: #4a5568;"
            "    font-size: 12px;"
            "    spacing: 4px;"
            "}"
            "QCheckBox::indicator {"
            "    width: 14px;"
            "    height: 14px;"
            "    border: 1px solid #cfd8e3;"
            "    border-radius: 3px;"
            "    background-color: white;"
            "}"
            "QCheckBox::indicator:checked {"
            "    background-color: #2ecc71;"
            "    border: 1px solid #27ae60;"
            "}"
        );

        int row = i / columns;
        int col = i % columns;
        grid->addWidget(cb, row, col);
        m_signals[psignals[i]] = cb;
    }

    outerLayout->addLayout(grid);
    return group;
}

void ManualDebug::setSignalActive(const QString &signalCode, bool active)
{
    if (m_signals.contains(signalCode)) {
        m_signals[signalCode]->setChecked(active);
    }
}

void ManualDebug::setSystemStatus(SystemStatus status)
{
    QAbstractButton *btn = m_statusGroup->button(status);
    if (btn) {
        btn->setChecked(true);
    }
}

void ManualDebug::PgSuccess()
{
    textedit->append("PG上电成功");
}

void ManualDebug::PgammaSuccess()
{
    textedit->append("pgamma完成");
}

void ManualDebug::LightUp()
{
    textedit->append("点亮信号");
}

void ManualDebug::PgConnect(bool ok)
{
    if(ok)
        textedit->append("PG连接成功");
    else
        textedit->append("PG连接断开");
}
