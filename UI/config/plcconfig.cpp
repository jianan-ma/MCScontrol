#include "plcconfig.h"
#include <QLabel>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>

PLCConfig::PLCConfig(QWidget *parent) : QWidget(parent),
    m_iniPath("config/plc.ini")
{
    setupUI();
    loadConfig();
}

void PLCConfig::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(12);

    // === PLC参数区域 ===
    QGroupBox *paramGroup = new QGroupBox("PLC参数", this);
    QHBoxLayout *paramLayout = new QHBoxLayout(paramGroup);
    paramLayout->setContentsMargins(20, 24, 20, 20);
    paramLayout->setSpacing(40);

    // 左列
    QFormLayout *leftLayout = new QFormLayout();
    leftLayout->setLabelAlignment(Qt::AlignRight);
    leftLayout->setSpacing(10);

    edtIP = new QLineEdit(paramGroup);
    edtIP->setFixedWidth(160);

    edtPLCStation = new QLineEdit(paramGroup);
    edtPLCStation->setFixedWidth(160);

    cbo = new QComboBox(paramGroup);
    cbo->addItem("R16ENCPU");
    cbo->setFixedWidth(160);

    // 心跳间隔 + 毫秒标签
    QWidget *heartWidget = new QWidget(paramGroup);
    QHBoxLayout *heartLayout = new QHBoxLayout(heartWidget);
    heartLayout->setContentsMargins(0, 0, 0, 0);
    heartLayout->setSpacing(6);
    edtHeartInterval = new QLineEdit(heartWidget);
    edtHeartInterval->setFixedWidth(120);
    QLabel *lbMs1 = new QLabel("毫秒", heartWidget);
    heartLayout->addWidget(edtHeartInterval);
    heartLayout->addWidget(lbMs1);
    heartLayout->addStretch();

    // 扫描间隔 + 毫秒标签
    QWidget *scanWidget = new QWidget(paramGroup);
    QHBoxLayout *scanLayout = new QHBoxLayout(scanWidget);
    scanLayout->setContentsMargins(0, 0, 0, 0);
    scanLayout->setSpacing(6);
    edtScanInterval = new QLineEdit(scanWidget);
    edtScanInterval->setFixedWidth(120);
    QLabel *lbMs2 = new QLabel("毫秒", scanWidget);
    scanLayout->addWidget(edtScanInterval);
    scanLayout->addWidget(lbMs2);
    scanLayout->addStretch();

    leftLayout->addRow("IP:", edtIP);
    leftLayout->addRow("PLC站点:", edtPLCStation);
    leftLayout->addRow("CPU类型:", cbo);
    leftLayout->addRow("发送心跳间隔:", heartWidget);
    leftLayout->addRow("扫描时间间隔:", scanWidget);

    // 右列
    QFormLayout *rightLayout = new QFormLayout();
    rightLayout->setLabelAlignment(Qt::AlignRight);
    rightLayout->setSpacing(10);

    edtNet = new QLineEdit(paramGroup);
    edtNet->setFixedWidth(160);

    edtPCStation = new QLineEdit(paramGroup);
    edtPCStation->setFixedWidth(160);

    edtGlassID = new QLineEdit(paramGroup);
    edtGlassID->setFixedWidth(160);

    // 心跳超时 + 毫秒标签
    QWidget *overtimeWidget = new QWidget(paramGroup);
    QHBoxLayout *overtimeLayout = new QHBoxLayout(overtimeWidget);
    overtimeLayout->setContentsMargins(0, 0, 0, 0);
    overtimeLayout->setSpacing(6);
    edtHeartOvertime = new QLineEdit(overtimeWidget);
    edtHeartOvertime->setFixedWidth(120);
    QLabel *lbMs3 = new QLabel("毫秒", overtimeWidget);
    overtimeLayout->addWidget(edtHeartOvertime);
    overtimeLayout->addWidget(lbMs3);
    overtimeLayout->addStretch();

    edtPCID = new QLineEdit(paramGroup);
    edtPCID->setFixedWidth(160);

    rightLayout->addRow("网络:", edtNet);
    rightLayout->addRow("PC站点:", edtPCStation);
    rightLayout->addRow("玻璃ID长度:", edtGlassID);
    rightLayout->addRow("接收心跳超时:", overtimeWidget);
    rightLayout->addRow("PC编号:", edtPCID);

    paramLayout->addLayout(leftLayout);
    paramLayout->addLayout(rightLayout);
    paramLayout->addStretch();

    mainLayout->addWidget(paramGroup);

    // === 配置文件路径区域 ===
    QGroupBox *fileGroup = new QGroupBox("配置文件", this);
    QHBoxLayout *fileLayout = new QHBoxLayout(fileGroup);
    fileLayout->setContentsMargins(20, 24, 20, 20);
    fileLayout->setSpacing(8);

    QLabel *pathLabel = new QLabel("INI路径:", fileGroup);
    edtPath = new QLineEdit(fileGroup);
    edtPath->setText(m_iniPath);
    btnLoad = new QPushButton("加载", fileGroup);
    btnSave = new QPushButton("保存", fileGroup);

    btnLoad->setFixedWidth(70);
    btnSave->setFixedWidth(70);

    fileLayout->addWidget(pathLabel);
    fileLayout->addWidget(edtPath, 1);
    fileLayout->addWidget(btnLoad);
    fileLayout->addWidget(btnSave);

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

    connect(btnLoad, &QPushButton::clicked, this, &PLCConfig::onbtnLoadClick);
    connect(btnSave, &QPushButton::clicked, this, &PLCConfig::onbtnSaveClick);
}

void PLCConfig::loadConfig(const QString &iniPath)
{
    QString path = iniPath.isEmpty() ? edtPath->text().trimmed() : iniPath;
    if (path.isEmpty()) {
        return;
    }
    m_iniPath = path;

    QSettings settings(path, QSettings::IniFormat);
    settings.beginGroup("PLC");

    edtIP->setText(settings.value("IP", "192.168.1.1").toString());
    edtPLCStation->setText(settings.value("PLCStation", "1").toString());
    cbo->setCurrentText(settings.value("CPUType", "R16ENCPU").toString());
    edtHeartInterval->setText(settings.value("HeartInterval", "1000").toString());
    edtScanInterval->setText(settings.value("ScanInterval", "500").toString());
    edtNet->setText(settings.value("Net", "255.255.255.0").toString());
    edtPCStation->setText(settings.value("PCStation", "2").toString());
    edtGlassID->setText(settings.value("GlassIDLength", "18").toString());
    edtHeartOvertime->setText(settings.value("HeartOvertime", "3000").toString());
    edtPCID->setText(settings.value("PCID", "1").toString());

    settings.endGroup();
}

void PLCConfig::saveConfig(const QString &iniPath)
{
    QString path = iniPath.isEmpty() ? edtPath->text().trimmed() : iniPath;
    if (path.isEmpty()) {
        QMessageBox::warning(this, "提示", "请先指定INI文件路径");
        return;
    }
    m_iniPath = path;

    QSettings settings(path, QSettings::IniFormat);
    settings.beginGroup("PLC");

    settings.setValue("IP", edtIP->text());
    settings.setValue("PLCStation", edtPLCStation->text());
    settings.setValue("CPUType", cbo->currentText());
    settings.setValue("HeartInterval", edtHeartInterval->text());
    settings.setValue("ScanInterval", edtScanInterval->text());
    settings.setValue("Net", edtNet->text());
    settings.setValue("PCStation", edtPCStation->text());
    settings.setValue("GlassIDLength", edtGlassID->text());
    settings.setValue("HeartOvertime", edtHeartOvertime->text());
    settings.setValue("PCID", edtPCID->text());

    settings.endGroup();
}

void PLCConfig::onbtnLoadClick()
{
    QString path = edtPath->text().trimmed();
    if (path.isEmpty()) {
        path = QFileDialog::getOpenFileName(this, "选择INI文件",
                                             QString(), "INI文件 (*.ini)");
        if (path.isEmpty()) return;
        edtPath->setText(path);
    }

    QFileInfo fi(path);
    if (!fi.exists()) {
        QMessageBox::warning(this, "提示", "文件不存在: " + path);
        return;
    }

    loadConfig(path);
}

void PLCConfig::onbtnSaveClick()
{
    QString path = edtPath->text().trimmed();
    if (path.isEmpty()) {
        path = QFileDialog::getSaveFileName(this, "保存INI文件",
                                              QString(), "INI文件 (*.ini)");
        if (path.isEmpty()) return;
        edtPath->setText(path);
    }

    saveConfig(path);
}
