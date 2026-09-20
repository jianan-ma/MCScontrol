#include "serialconfig.h"
#include <QSerialPortInfo>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>

SerialConfig::SerialConfig(QWidget *parent) :
    QWidget(parent),
    m_iniPath("config/serial.ini")
{
    setupUI();
    initControls();
    loadConfig();
}

void SerialConfig::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(12);

    // === 串口参数区域 ===
    QGroupBox *paramGroup = new QGroupBox("串口参数", this);
    QFormLayout *paramLayout = new QFormLayout(paramGroup);
    paramLayout->setLabelAlignment(Qt::AlignRight);
    paramLayout->setSpacing(10);
    paramLayout->setContentsMargins(20, 24, 20, 20);

    m_cboPortName = new QComboBox(paramGroup);
    m_cboBaudRate = new QComboBox(paramGroup);
    m_cboDataBits = new QComboBox(paramGroup);
    m_cboParity = new QComboBox(paramGroup);
    m_cboStopBits = new QComboBox(paramGroup);
    m_cboFlowControl = new QComboBox(paramGroup);

    // 统一控件宽度
    int cboWidth = 200;
    m_cboPortName->setFixedWidth(cboWidth);
    m_cboBaudRate->setFixedWidth(cboWidth);
    m_cboDataBits->setFixedWidth(cboWidth);
    m_cboParity->setFixedWidth(cboWidth);
    m_cboStopBits->setFixedWidth(cboWidth);
    m_cboFlowControl->setFixedWidth(cboWidth);

    paramLayout->addRow("串口号:", m_cboPortName);
    paramLayout->addRow("波特率:", m_cboBaudRate);
    paramLayout->addRow("数据位:", m_cboDataBits);
    paramLayout->addRow("校验位:", m_cboParity);
    paramLayout->addRow("停止位:", m_cboStopBits);
    paramLayout->addRow("流控制:", m_cboFlowControl);

    mainLayout->addWidget(paramGroup);

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

    connect(m_btnLoad, &QPushButton::clicked, this, &SerialConfig::onBtnLoadClick);
    connect(m_btnSave, &QPushButton::clicked, this, &SerialConfig::onBtnSaveClick);
}

void SerialConfig::initControls()
{
    // 串口号 - 枚举可用串口
    m_cboPortName->clear();
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &port : ports) {
        m_cboPortName->addItem(port.portName());
    }
    if (m_cboPortName->count() == 0) {
        m_cboPortName->addItem("COM1");
    }

    // 波特率
    m_cboBaudRate->clear();
    QStringList baudRates = {"9600", "19200", "38400", "57600", "115200", "230400", "460800", "921600"};
    m_cboBaudRate->addItems(baudRates);
    m_cboBaudRate->setCurrentText("115200");

    // 数据位
    m_cboDataBits->clear();
    m_cboDataBits->addItems({"5", "6", "7", "8"});
    m_cboDataBits->setCurrentText("8");

    // 校验位
    m_cboParity->clear();
    m_cboParity->addItems({"无", "偶", "奇", "标记", "空格"});
    m_cboParity->setCurrentIndex(0);

    // 停止位
    m_cboStopBits->clear();
    m_cboStopBits->addItems({"1", "1.5", "2"});
    m_cboStopBits->setCurrentText("1");

    // 流控制
    m_cboFlowControl->clear();
    m_cboFlowControl->addItems({"无", "硬件", "软件"});
    m_cboFlowControl->setCurrentIndex(0);
}

void SerialConfig::loadConfig(const QString &iniPath)
{
    QString path = iniPath.isEmpty() ? m_edtPath->text().trimmed() : iniPath;
    if (path.isEmpty()) {
        return;
    }
    m_iniPath = path;

    QSettings settings(path, QSettings::IniFormat);
    settings.beginGroup("Serial");

    QString port = settings.value("PortName", m_cboPortName->currentText()).toString();
    int baud = settings.value("BaudRate", m_cboBaudRate->currentText().toInt()).toInt();
    int data = settings.value("DataBits", m_cboDataBits->currentText().toInt()).toInt();
    QString parity = settings.value("Parity", m_cboParity->currentText()).toString();
    QString stop = settings.value("StopBits", m_cboStopBits->currentText()).toString();
    QString flow = settings.value("FlowControl", m_cboFlowControl->currentText()).toString();

    settings.endGroup();

    // 应用到界面
    int idx = m_cboPortName->findText(port);
    if (idx >= 0) {
        m_cboPortName->setCurrentIndex(idx);
    }

    idx = m_cboBaudRate->findText(QString::number(baud));
    if (idx >= 0) {
        m_cboBaudRate->setCurrentIndex(idx);
    }

    idx = m_cboDataBits->findText(QString::number(data));
    if (idx >= 0) {
        m_cboDataBits->setCurrentIndex(idx);
    }

    idx = m_cboParity->findText(parity);
    if (idx >= 0) {
        m_cboParity->setCurrentIndex(idx);
    }

    idx = m_cboStopBits->findText(stop);
    if (idx >= 0) {
        m_cboStopBits->setCurrentIndex(idx);
    }

    idx = m_cboFlowControl->findText(flow);
    if (idx >= 0) {
        m_cboFlowControl->setCurrentIndex(idx);
    }
}

void SerialConfig::saveConfig(const QString &iniPath)
{
    QString path = iniPath.isEmpty() ? m_edtPath->text().trimmed() : iniPath;
    if (path.isEmpty()) {
        QMessageBox::warning(this, "提示", "请先指定INI文件路径");
        return;
    }
    m_iniPath = path;

    QSettings settings(path, QSettings::IniFormat);
    settings.beginGroup("Serial");

    settings.setValue("PortName", m_cboPortName->currentText());
    settings.setValue("BaudRate", m_cboBaudRate->currentText().toInt());
    settings.setValue("DataBits", m_cboDataBits->currentText().toInt());
    settings.setValue("Parity", m_cboParity->currentText());
    settings.setValue("StopBits", m_cboStopBits->currentText());
    settings.setValue("FlowControl", m_cboFlowControl->currentText());

    settings.endGroup();
}

QString SerialConfig::portName() const
{
    return m_cboPortName->currentText();
}

int SerialConfig::baudRate() const
{
    return m_cboBaudRate->currentText().toInt();
}

int SerialConfig::dataBits() const
{
    return m_cboDataBits->currentText().toInt();
}

QString SerialConfig::parity() const
{
    return m_cboParity->currentText();
}

int SerialConfig::stopBits() const
{
    QString stop = m_cboStopBits->currentText();
    if (stop == "1") return 1;
    if (stop == "1.5") return 3;
    if (stop == "2") return 2;
    return 1;
}

QString SerialConfig::flowControl() const
{
    return m_cboFlowControl->currentText();
}

void SerialConfig::onBtnLoadClick()
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

void SerialConfig::onBtnSaveClick()
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
