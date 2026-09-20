#include "socketconfig.h"
#include <QLabel>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>

SocketConfig::SocketConfig(QWidget *parent) :
    QWidget(parent),
    m_iniPath("config/socket.ini")
{
    setupUI();
    loadConfig();
}

void SocketConfig::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(12);

    // === 连接参数区域 ===
    QGroupBox *connGroup = new QGroupBox("连接参数", this);
    QFormLayout *connLayout = new QFormLayout(connGroup);
    connLayout->setLabelAlignment(Qt::AlignRight);
    connLayout->setSpacing(10);
    connLayout->setContentsMargins(20, 24, 20, 20);

    m_cboType = new QComboBox(connGroup);
    m_cboType->addItems({"TCP Server", "TCP Client", "UDP"});
    m_cboType->setFixedWidth(200);

    m_edtLocalIP = new QLineEdit(connGroup);
    m_edtLocalIP->setFixedWidth(200);

    m_edtLocalPort = new QLineEdit(connGroup);
    m_edtLocalPort->setFixedWidth(200);

    m_edtRemoteIP = new QLineEdit(connGroup);
    m_edtRemoteIP->setFixedWidth(200);

    m_edtRemotePort = new QLineEdit(connGroup);
    m_edtRemotePort->setFixedWidth(200);

    connLayout->addRow("连接类型:", m_cboType);
    connLayout->addRow("本地IP:", m_edtLocalIP);
    connLayout->addRow("本地端口:", m_edtLocalPort);
    connLayout->addRow("远程IP:", m_edtRemoteIP);
    connLayout->addRow("远程端口:", m_edtRemotePort);

    mainLayout->addWidget(connGroup);

    // === 超时与重连区域 ===
    QGroupBox *timeoutGroup = new QGroupBox("超时与重连", this);
    QFormLayout *timeoutLayout = new QFormLayout(timeoutGroup);
    timeoutLayout->setLabelAlignment(Qt::AlignRight);
    timeoutLayout->setSpacing(10);
    timeoutLayout->setContentsMargins(20, 24, 20, 20);

    int fieldWidth = 200;

    // 连接超时
    QWidget *connTimeoutWidget = new QWidget(timeoutGroup);
    QHBoxLayout *connTimeoutLayout = new QHBoxLayout(connTimeoutWidget);
    connTimeoutLayout->setContentsMargins(0, 0, 0, 0);
    connTimeoutLayout->setSpacing(6);
    m_edtConnectTimeout = new QLineEdit(connTimeoutWidget);
    m_edtConnectTimeout->setFixedWidth(140);
    QLabel *lbMs1 = new QLabel("毫秒", connTimeoutWidget);
    connTimeoutLayout->addWidget(m_edtConnectTimeout);
    connTimeoutLayout->addWidget(lbMs1);
    connTimeoutLayout->addStretch();

    // 读取超时
    QWidget *readTimeoutWidget = new QWidget(timeoutGroup);
    QHBoxLayout *readTimeoutLayout = new QHBoxLayout(readTimeoutWidget);
    readTimeoutLayout->setContentsMargins(0, 0, 0, 0);
    readTimeoutLayout->setSpacing(6);
    m_edtReadTimeout = new QLineEdit(readTimeoutWidget);
    m_edtReadTimeout->setFixedWidth(140);
    QLabel *lbMs2 = new QLabel("毫秒", readTimeoutWidget);
    readTimeoutLayout->addWidget(m_edtReadTimeout);
    readTimeoutLayout->addWidget(lbMs2);
    readTimeoutLayout->addStretch();

    // 重连间隔
    QWidget *reconnectWidget = new QWidget(timeoutGroup);
    QHBoxLayout *reconnectLayout = new QHBoxLayout(reconnectWidget);
    reconnectLayout->setContentsMargins(0, 0, 0, 0);
    reconnectLayout->setSpacing(6);
    m_edtReconnectInterval = new QLineEdit(reconnectWidget);
    m_edtReconnectInterval->setFixedWidth(140);
    QLabel *lbMs3 = new QLabel("毫秒", reconnectWidget);
    reconnectLayout->addWidget(m_edtReconnectInterval);
    reconnectLayout->addWidget(lbMs3);
    reconnectLayout->addStretch();

    timeoutLayout->addRow("连接超时:", connTimeoutWidget);
    timeoutLayout->addRow("读取超时:", readTimeoutWidget);
    timeoutLayout->addRow("重连间隔:", reconnectWidget);

    mainLayout->addWidget(timeoutGroup);

    // === 配置文件路径区域 ===
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

    // 统一样式
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

    connect(m_btnLoad, &QPushButton::clicked, this, &SocketConfig::onBtnLoadClick);
    connect(m_btnSave, &QPushButton::clicked, this, &SocketConfig::onBtnSaveClick);
}

void SocketConfig::loadConfig(const QString &iniPath)
{
    QString path = iniPath.isEmpty() ? m_edtPath->text().trimmed() : iniPath;
    if (path.isEmpty()) {
        return;
    }
    m_iniPath = path;

    QSettings settings(path, QSettings::IniFormat);
    settings.beginGroup("Socket");

    QString type = settings.value("Type", "TCP Client").toString();
    int idx = m_cboType->findText(type);
    if (idx >= 0) {
        m_cboType->setCurrentIndex(idx);
    }

    m_edtLocalIP->setText(settings.value("LocalIP", "0.0.0.0").toString());
    m_edtLocalPort->setText(settings.value("LocalPort", "8080").toString());
    m_edtRemoteIP->setText(settings.value("RemoteIP", "127.0.0.1").toString());
    m_edtRemotePort->setText(settings.value("RemotePort", "8080").toString());
    m_edtConnectTimeout->setText(settings.value("ConnectTimeout", "3000").toString());
    m_edtReadTimeout->setText(settings.value("ReadTimeout", "3000").toString());
    m_edtReconnectInterval->setText(settings.value("ReconnectInterval", "5000").toString());

    settings.endGroup();
}

void SocketConfig::saveConfig(const QString &iniPath)
{
    QString path = iniPath.isEmpty() ? m_edtPath->text().trimmed() : iniPath;
    if (path.isEmpty()) {
        QMessageBox::warning(this, "提示", "请先指定INI文件路径");
        return;
    }
    m_iniPath = path;

    QSettings settings(path, QSettings::IniFormat);
    settings.beginGroup("Socket");

    settings.setValue("Type", m_cboType->currentText());
    settings.setValue("LocalIP", m_edtLocalIP->text());
    settings.setValue("LocalPort", m_edtLocalPort->text());
    settings.setValue("RemoteIP", m_edtRemoteIP->text());
    settings.setValue("RemotePort", m_edtRemotePort->text());
    settings.setValue("ConnectTimeout", m_edtConnectTimeout->text());
    settings.setValue("ReadTimeout", m_edtReadTimeout->text());
    settings.setValue("ReconnectInterval", m_edtReconnectInterval->text());

    settings.endGroup();
}

void SocketConfig::onBtnLoadClick()
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

void SocketConfig::onBtnSaveClick()
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
