#include "systemconfig.h"
#include <QLabel>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>

SystemConfig::SystemConfig(QWidget *parent) :
    QWidget(parent),
    m_iniPath("config/system.ini")
{
    setupUI();
    loadConfig();
}

void SystemConfig::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(12);

    // === 系统设置 ===
    QGroupBox *sysGroup = new QGroupBox("系统设置", this);
    QFormLayout *sysLayout = new QFormLayout(sysGroup);
    sysLayout->setLabelAlignment(Qt::AlignRight);
    sysLayout->setSpacing(10);
    sysLayout->setContentsMargins(20, 24, 20, 20);

    m_edtSystemName = new QLineEdit(sysGroup);
    m_edtSystemName->setFixedWidth(200);

    m_cboLanguage = new QComboBox(sysGroup);
    m_cboLanguage->addItems({"简体中文", "English"});
    m_cboLanguage->setFixedWidth(200);

    m_cboTheme = new QComboBox(sysGroup);
    m_cboTheme->addItems({"浅色", "深色"});
    m_cboTheme->setFixedWidth(200);

    m_chkAutoStart = new QCheckBox("开机自动启动", sysGroup);
    m_chkAutoLogin = new QCheckBox("自动登录", sysGroup);

    sysLayout->addRow("系统名称:", m_edtSystemName);
    sysLayout->addRow("界面语言:", m_cboLanguage);
    sysLayout->addRow("界面主题:", m_cboTheme);
    sysLayout->addRow("", m_chkAutoStart);
    sysLayout->addRow("", m_chkAutoLogin);

    mainLayout->addWidget(sysGroup);

    // === 路径设置 ===
    QGroupBox *pathGroup = new QGroupBox("路径设置", this);
    QFormLayout *pathLayout = new QFormLayout(pathGroup);
    pathLayout->setLabelAlignment(Qt::AlignRight);
    pathLayout->setSpacing(10);
    pathLayout->setContentsMargins(20, 24, 20, 20);

    m_edtDataPath = new QLineEdit(pathGroup);
    m_edtLogPath = new QLineEdit(pathGroup);
    m_edtRecipePath = new QLineEdit(pathGroup);
    m_edtReportPath = new QLineEdit(pathGroup);

    pathLayout->addRow("数据路径:", m_edtDataPath);
    pathLayout->addRow("日志路径:", m_edtLogPath);
    pathLayout->addRow("配方路径:", m_edtRecipePath);
    pathLayout->addRow("报表路径:", m_edtReportPath);

    mainLayout->addWidget(pathGroup);

    // === 数据库设置 ===
    QGroupBox *dbGroup = new QGroupBox("数据库设置", this);
    QHBoxLayout *dbLayout = new QHBoxLayout(dbGroup);
    dbLayout->setContentsMargins(20, 24, 20, 20);
    dbLayout->setSpacing(40);

    // 左列
    QFormLayout *dbLeft = new QFormLayout();
    dbLeft->setLabelAlignment(Qt::AlignRight);
    dbLeft->setSpacing(10);

    m_edtDbHost = new QLineEdit(dbGroup);
    m_edtDbHost->setFixedWidth(180);
    m_edtDbPort = new QLineEdit(dbGroup);
    m_edtDbPort->setFixedWidth(180);
    m_edtDbName = new QLineEdit(dbGroup);
    m_edtDbName->setFixedWidth(180);

    dbLeft->addRow("数据库地址:", m_edtDbHost);
    dbLeft->addRow("端口:", m_edtDbPort);
    dbLeft->addRow("数据库名:", m_edtDbName);

    // 右列
    QFormLayout *dbRight = new QFormLayout();
    dbRight->setLabelAlignment(Qt::AlignRight);
    dbRight->setSpacing(10);

    m_edtDbUser = new QLineEdit(dbGroup);
    m_edtDbUser->setFixedWidth(180);
    m_edtDbPassword = new QLineEdit(dbGroup);
    m_edtDbPassword->setEchoMode(QLineEdit::Password);
    m_edtDbPassword->setFixedWidth(180);

    dbRight->addRow("用户名:", m_edtDbUser);
    dbRight->addRow("密码:", m_edtDbPassword);

    dbLayout->addLayout(dbLeft);
    dbLayout->addLayout(dbRight);
    dbLayout->addStretch();

    mainLayout->addWidget(dbGroup);

    // === 运行参数 ===
    QGroupBox *runGroup = new QGroupBox("运行参数", this);
    QFormLayout *runLayout = new QFormLayout(runGroup);
    runLayout->setLabelAlignment(Qt::AlignRight);
    runLayout->setSpacing(10);
    runLayout->setContentsMargins(20, 24, 20, 20);

    // 心跳间隔
    QWidget *hbWidget = new QWidget(runGroup);
    QHBoxLayout *hbLayout = new QHBoxLayout(hbWidget);
    hbLayout->setContentsMargins(0, 0, 0, 0);
    hbLayout->setSpacing(6);
    m_edtHeartbeatInterval = new QLineEdit(hbWidget);
    m_edtHeartbeatInterval->setFixedWidth(140);
    QLabel *lbMs1 = new QLabel("毫秒", hbWidget);
    hbLayout->addWidget(m_edtHeartbeatInterval);
    hbLayout->addWidget(lbMs1);
    hbLayout->addStretch();

    // 扫描间隔
    QWidget *scanWidget = new QWidget(runGroup);
    QHBoxLayout *scanLayout = new QHBoxLayout(scanWidget);
    scanLayout->setContentsMargins(0, 0, 0, 0);
    scanLayout->setSpacing(6);
    m_edtScanInterval = new QLineEdit(scanWidget);
    m_edtScanInterval->setFixedWidth(140);
    QLabel *lbMs2 = new QLabel("毫秒", scanWidget);
    scanLayout->addWidget(m_edtScanInterval);
    scanLayout->addWidget(lbMs2);
    scanLayout->addStretch();

    // 日志保留天数
    QWidget *daysWidget = new QWidget(runGroup);
    QHBoxLayout *daysLayout = new QHBoxLayout(daysWidget);
    daysLayout->setContentsMargins(0, 0, 0, 0);
    daysLayout->setSpacing(6);
    m_edtLogKeepDays = new QLineEdit(daysWidget);
    m_edtLogKeepDays->setFixedWidth(140);
    QLabel *lbDays = new QLabel("天", daysWidget);
    daysLayout->addWidget(m_edtLogKeepDays);
    daysLayout->addWidget(lbDays);
    daysLayout->addStretch();

    m_chkSaveLog = new QCheckBox("启用日志记录", runGroup);

    runLayout->addRow("心跳间隔:", hbWidget);
    runLayout->addRow("扫描间隔:", scanWidget);
    runLayout->addRow("日志保留:", daysWidget);
    runLayout->addRow("", m_chkSaveLog);

    mainLayout->addWidget(runGroup);

    // === 配置文件路径 ===
    QGroupBox *fileGroup = new QGroupBox("配置文件", this);
    QHBoxLayout *fileLayout = new QHBoxLayout(fileGroup);
    fileLayout->setContentsMargins(20, 24, 20, 20);
    fileLayout->setSpacing(8);

    QLabel *pathLabel = new QLabel("INI路径:", fileGroup);
    m_edtPath = new QLineEdit(fileGroup);
    m_edtPath->setText(m_iniPath);
    m_btnLoad = new QPushButton("加载", fileGroup);
    m_btnSave = new QPushButton("保存", fileGroup);

    m_btnLoad->setFixedWidth(70);
    m_btnSave->setFixedWidth(70);

    fileLayout->addWidget(pathLabel);
    fileLayout->addWidget(m_edtPath, 1);
    fileLayout->addWidget(m_btnLoad);
    fileLayout->addWidget(m_btnSave);

    mainLayout->addWidget(fileGroup);
    mainLayout->addStretch();

    applyStyle();

    connect(m_btnLoad, &QPushButton::clicked, this, &SystemConfig::onBtnLoadClick);
    connect(m_btnSave, &QPushButton::clicked, this, &SystemConfig::onBtnSaveClick);
}

void SystemConfig::applyStyle()
{
    setStyleSheet(
        "QGroupBox {"
        "    border: 1px solid #d0d7de;"
        "    border-radius: 6px;"
        "    margin-top: 12px;"
        "    font-size: 13px;"
        "    font-weight: bold;"
        "    color: #4a5568;"
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
        "QComboBox {"
        "    border: 1px solid #cfd8e3;"
        "    border-radius: 4px;"
        "    padding: 4px 8px;"
        "    font-size: 13px;"
        "    color: #1a1a2e;"
        "}"
        "QComboBox:hover {"
        "    border: 1px solid #4a90d9;"
        "}"
        "QComboBox::drop-down {"
        "    border: none;"
        "    width: 22px;"
        "}"
        "QCheckBox {"
        "    color: #1a1a2e;"
        "    font-size: 13px;"
        "    spacing: 6px;"
        "}"
        "QCheckBox::indicator {"
        "    width: 16px;"
        "    height: 16px;"
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
    );
}

void SystemConfig::loadConfig(const QString &iniPath)
{
    QString path = iniPath.isEmpty() ? m_edtPath->text().trimmed() : iniPath;
    if (path.isEmpty()) {
        return;
    }
    m_iniPath = path;

    QSettings settings(path, QSettings::IniFormat);

    // [System]
    settings.beginGroup("System");
    m_edtSystemName->setText(settings.value("SystemName", "MCS").toString());
    m_cboLanguage->setCurrentText(settings.value("Language", "简体中文").toString());
    m_cboTheme->setCurrentText(settings.value("Theme", "浅色").toString());
    m_chkAutoStart->setChecked(settings.value("AutoStart", false).toBool());
    m_chkAutoLogin->setChecked(settings.value("AutoLogin", false).toBool());
    settings.endGroup();

    // [Path]
    settings.beginGroup("Path");
    m_edtDataPath->setText(settings.value("DataPath", "./data").toString());
    m_edtLogPath->setText(settings.value("LogPath", "./logs").toString());
    m_edtRecipePath->setText(settings.value("RecipePath", "./recipes").toString());
    m_edtReportPath->setText(settings.value("ReportPath", "./reports").toString());
    settings.endGroup();

    // [Database]
    settings.beginGroup("Database");
    m_edtDbHost->setText(settings.value("Host", "127.0.0.1").toString());
    m_edtDbPort->setText(settings.value("Port", "3306").toString());
    m_edtDbName->setText(settings.value("Name", "mcs_db").toString());
    m_edtDbUser->setText(settings.value("User", "root").toString());
    m_edtDbPassword->setText(settings.value("Password", "").toString());
    settings.endGroup();

    // [Run]
    settings.beginGroup("Run");
    m_edtHeartbeatInterval->setText(settings.value("HeartbeatInterval", "1000").toString());
    m_edtScanInterval->setText(settings.value("ScanInterval", "500").toString());
    m_edtLogKeepDays->setText(settings.value("LogKeepDays", "30").toString());
    m_chkSaveLog->setChecked(settings.value("SaveLog", true).toBool());
    settings.endGroup();
}

void SystemConfig::saveConfig(const QString &iniPath)
{
    QString path = iniPath.isEmpty() ? m_edtPath->text().trimmed() : iniPath;
    if (path.isEmpty()) {
        QMessageBox::warning(this, "提示", "请先指定INI文件路径");
        return;
    }
    m_iniPath = path;

    QSettings settings(path, QSettings::IniFormat);

    // [System]
    settings.beginGroup("System");
    settings.setValue("SystemName", m_edtSystemName->text());
    settings.setValue("Language", m_cboLanguage->currentText());
    settings.setValue("Theme", m_cboTheme->currentText());
    settings.setValue("AutoStart", m_chkAutoStart->isChecked());
    settings.setValue("AutoLogin", m_chkAutoLogin->isChecked());
    settings.endGroup();

    // [Path]
    settings.beginGroup("Path");
    settings.setValue("DataPath", m_edtDataPath->text());
    settings.setValue("LogPath", m_edtLogPath->text());
    settings.setValue("RecipePath", m_edtRecipePath->text());
    settings.setValue("ReportPath", m_edtReportPath->text());
    settings.endGroup();

    // [Database]
    settings.beginGroup("Database");
    settings.setValue("Host", m_edtDbHost->text());
    settings.setValue("Port", m_edtDbPort->text());
    settings.setValue("Name", m_edtDbName->text());
    settings.setValue("User", m_edtDbUser->text());
    settings.setValue("Password", m_edtDbPassword->text());
    settings.endGroup();

    // [Run]
    settings.beginGroup("Run");
    settings.setValue("HeartbeatInterval", m_edtHeartbeatInterval->text());
    settings.setValue("ScanInterval", m_edtScanInterval->text());
    settings.setValue("LogKeepDays", m_edtLogKeepDays->text());
    settings.setValue("SaveLog", m_chkSaveLog->isChecked());
    settings.endGroup();
}

void SystemConfig::onBtnLoadClick()
{
    QString path = m_edtPath->text().trimmed();
    if (path.isEmpty()) {
        path = QFileDialog::getOpenFileName(this, "选择INI文件",
                                             QString(), "INI文件 (*.ini)");
        if (path.isEmpty()) return;
        m_edtPath->setText(path);
    }

    QFileInfo fi(path);
    if (!fi.exists()) {
        QMessageBox::warning(this, "提示", "文件不存在: " + path);
        return;
    }

    loadConfig(path);
}

void SystemConfig::onBtnSaveClick()
{
    QString path = m_edtPath->text().trimmed();
    if (path.isEmpty()) {
        path = QFileDialog::getSaveFileName(this, "保存INI文件",
                                              QString(), "INI文件 (*.ini)");
        if (path.isEmpty()) return;
        m_edtPath->setText(path);
    }

    saveConfig(path);
}
