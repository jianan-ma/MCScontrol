#include "logquery.h"
#include <QHeaderView>
#include <QDateTime>
#include <QSizePolicy>
#include <QCheckBox>

// ============================================================================
// 前向声明 Database 单例
// 用户后续实现 Database 类后，取消下方注释并 #include "database.h"
// ============================================================================

// 数据库单例的接口声明（用户后续实现）
struct DatabaseLogRecord {
    int         id;
    int         type;           // 0=Error, 1=Warning, 2=Info
    QDateTime   timestamp;
    QString     stationNumber;
    QString     message;
};

// 假设的 Database 单例接口
// 用户实现后替换此处声明
class Database
{
public:
    static Database *instance();

    QVector<DatabaseLogRecord> queryLogs(
        const QList<int>    &typeFilter,
        const QDateTime     &startTime,
        const QDateTime     &endTime,
        const QString       &stationNumber)
    {
        // TODO: 用户实现数据库查询
        Q_UNUSED(typeFilter)
        Q_UNUSED(startTime)
        Q_UNUSED(endTime)
        Q_UNUSED(stationNumber)
        return QVector<DatabaseLogRecord>();
    }
};

// 简单单例实现（用户后续替换为真实实现）
Database *Database::instance()
{
    static Database instance;
    return &instance;
}

// ============================================================================

LogQuery::LogQuery(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void LogQuery::setupUI()
{
    setStyleSheet(
        "LogQuery {"
        "    background-color: #ffffff;"
        "    border: 1px solid #e0e6ed;"
        "    border-radius: 8px;"
        "}"
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // === 顶部栏 ===
    m_headerWidget = new QWidget(this);
    m_headerWidget->setFixedHeight(48);
    m_headerWidget->setStyleSheet(
        "QWidget {"
        "    background-color: #f0f4f8;"
        "    border-top-left-radius: 8px;"
        "    border-top-right-radius: 8px;"
        "    border-bottom: 1px solid #e0e6ed;"
        "}"
    );

    QHBoxLayout *headerLayout = new QHBoxLayout(m_headerWidget);
    headerLayout->setContentsMargins(12, 6, 12, 6);
    headerLayout->setSpacing(8);

    // 折叠按钮
//    m_collapseBtn = new QPushButton(m_headerWidget);
//    m_collapseBtn->setFixedSize(28, 28);
//    m_collapseBtn->setText("▼");
//    m_collapseBtn->setCursor(Qt::PointingHandCursor);
//    m_collapseBtn->setStyleSheet(
//        "QPushButton {"
//        "    background-color: transparent;"
//        "    border: none;"
//        "    border-radius: 4px;"
//        "    color: #4a5568;"
//        "    font-size: 12px;"
//        "}"
//        "QPushButton:hover {"
//        "    background-color: rgba(0, 0, 0, 10);"
//        "}"
//    );
//    connect(m_collapseBtn, &QPushButton::clicked, this, &LogQuery::onCollapseClicked);

    // 标题
    m_titleLabel = new QLabel("日志查询", m_headerWidget);
    m_titleLabel->setStyleSheet(
        "color: #1a1a2e;"
        "font-size: 14px;"
        "font-weight: bold;"
    );

//    headerLayout->addWidget(m_collapseBtn);
    headerLayout->addWidget(m_titleLabel);
    headerLayout->addSpacing(12);

    // 错误类型筛选按钮
    m_errorBtn = new QPushButton("错误", m_headerWidget);
    m_errorBtn->setCheckable(true);
    m_errorBtn->setChecked(true);
    m_errorBtn->setCursor(Qt::PointingHandCursor);
    m_errorBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: #e8f5e9;"
        "    color: #c62828;"
        "    border: 1px solid #ef9a9a;"
        "    border-radius: 4px;"
        "    padding: 4px 12px;"
        "    font-size: 12px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #ffebee;"
        "}"
        "QPushButton:checked {"
        "    background-color: #c62828;"
        "    color: white;"
        "    border: 1px solid #c62828;"
        "}"
    );

    m_warningBtn = new QPushButton("警告", m_headerWidget);
    m_warningBtn->setCheckable(true);
    m_warningBtn->setChecked(true);
    m_warningBtn->setCursor(Qt::PointingHandCursor);
    m_warningBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: #fff8e1;"
        "    color: #f57f17;"
        "    border: 1px solid #ffe082;"
        "    border-radius: 4px;"
        "    padding: 4px 12px;"
        "    font-size: 12px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #fff3e0;"
        "}"
        "QPushButton:checked {"
        "    background-color: #f57f17;"
        "    color: white;"
        "    border: 1px solid #f57f17;"
        "}"
    );

    m_infoBtn = new QPushButton("信息", m_headerWidget);
    m_infoBtn->setCheckable(true);
    m_infoBtn->setChecked(true);
    m_infoBtn->setCursor(Qt::PointingHandCursor);
    m_infoBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: #e3f2fd;"
        "    color: #1565c0;"
        "    border: 1px solid #90caf9;"
        "    border-radius: 4px;"
        "    padding: 4px 12px;"
        "    font-size: 12px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #e1f5fe;"
        "}"
        "QPushButton:checked {"
        "    background-color: #1565c0;"
        "    color: white;"
        "    border: 1px solid #1565c0;"
        "}"
    );

    connect(m_errorBtn, &QPushButton::toggled, this, &LogQuery::onFilterChanged);
    connect(m_warningBtn, &QPushButton::toggled, this, &LogQuery::onFilterChanged);
    connect(m_infoBtn, &QPushButton::toggled, this, &LogQuery::onFilterChanged);

    headerLayout->addWidget(m_errorBtn);
    headerLayout->addWidget(m_warningBtn);
    headerLayout->addWidget(m_infoBtn);
    headerLayout->addSpacing(8);

    // === 时间筛选 ===
    m_chkTimeFilter = new QCheckBox("时间", m_headerWidget);
    m_chkTimeFilter->setStyleSheet(
        "QCheckBox {"
        "    color: #4a5568;"
        "    font-size: 12px;"
        "    spacing: 4px;"
        "}"
    );
    m_chkTimeFilter->setChecked(false);
    headerLayout->addWidget(m_chkTimeFilter);

    m_startTimeEdit = new QDateTimeEdit(m_headerWidget);
    m_startTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    m_startTimeEdit->setCalendarPopup(true);
    m_startTimeEdit->setFixedHeight(28);
    m_startTimeEdit->setFixedWidth(170);
    m_startTimeEdit->setDateTime(QDateTime::currentDateTime().addDays(-1));
    m_startTimeEdit->setStyleSheet(
        "QDateTimeEdit {"
        "    background-color: white;"
        "    border: 1px solid #cfd8e3;"
        "    border-radius: 4px;"
        "    padding: 0 6px;"
        "    font-size: 12px;"
        "    color: #1a1a2e;"
        "}"
        "QDateTimeEdit:focus {"
        "    border: 1px solid #4a90d9;"
        "}"
    );
    m_startTimeEdit->setEnabled(false);

    QLabel *timeSep = new QLabel("~", m_headerWidget);
    timeSep->setStyleSheet("color: #4a5568; font-size: 12px;");

    m_endTimeEdit = new QDateTimeEdit(m_headerWidget);
    m_endTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    m_endTimeEdit->setCalendarPopup(true);
    m_endTimeEdit->setFixedHeight(28);
    m_endTimeEdit->setFixedWidth(170);
    m_endTimeEdit->setDateTime(QDateTime::currentDateTime());
    m_endTimeEdit->setStyleSheet(
        "QDateTimeEdit {"
        "    background-color: white;"
        "    border: 1px solid #cfd8e3;"
        "    border-radius: 4px;"
        "    padding: 0 6px;"
        "    font-size: 12px;"
        "    color: #1a1a2e;"
        "}"
        "QDateTimeEdit:focus {"
        "    border: 1px solid #4a90d9;"
        "}"
    );
    m_endTimeEdit->setEnabled(false);

    connect(m_chkTimeFilter, &QCheckBox::toggled, this, [this](bool checked) {
        m_startTimeEdit->setEnabled(checked);
        m_endTimeEdit->setEnabled(checked);
    });
    connect(m_chkTimeFilter, &QCheckBox::toggled, this, &LogQuery::onTimeFilterChanged);
    connect(m_startTimeEdit, &QDateTimeEdit::dateTimeChanged, this, &LogQuery::onTimeFilterChanged);
    connect(m_endTimeEdit, &QDateTimeEdit::dateTimeChanged, this, &LogQuery::onTimeFilterChanged);

    headerLayout->addWidget(m_startTimeEdit);
    headerLayout->addWidget(timeSep);
    headerLayout->addWidget(m_endTimeEdit);

    // 查询按钮
    m_searchBtn = new QPushButton("查询", m_headerWidget);
    m_searchBtn->setFixedSize(60, 28);
    m_searchBtn->setCursor(Qt::PointingHandCursor);
    m_searchBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: #4a90d9;"
        "    border: none;"
        "    border-radius: 4px;"
        "    color: white;"
        "    font-size: 12px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #357abd;"
        "}"
    );
    connect(m_searchBtn, &QPushButton::clicked, this, &LogQuery::onQueryClicked);

    headerLayout->addWidget(m_searchBtn);
    headerLayout->addStretch();

    // 工位编号筛选输入框
    m_stationEdit = new QLineEdit(m_headerWidget);
    m_stationEdit->setPlaceholderText("输入工位编号筛选");
    m_stationEdit->setFixedHeight(28);
    m_stationEdit->setFixedWidth(160);
    m_stationEdit->setStyleSheet(
        "QLineEdit {"
        "    background-color: white;"
        "    border: 1px solid #cfd8e3;"
        "    border-radius: 4px;"
        "    padding: 0 8px;"
        "    font-size: 12px;"
        "    color: #1a1a2e;"
        "}"
        "QLineEdit:focus {"
        "    border: 1px solid #4a90d9;"
        "}"
    );
    connect(m_stationEdit, &QLineEdit::returnPressed,
            this, &LogQuery::onStationFilterChanged);

    headerLayout->addWidget(m_stationEdit);

    mainLayout->addWidget(m_headerWidget);

    // === 日志表格 ===
    m_tableContainer = new QWidget(this);
    m_tableContainer->setStyleSheet(
        "QWidget {"
        "    background-color: white;"
        "    border-bottom-left-radius: 8px;"
        "    border-bottom-right-radius: 8px;"
        "}"
    );

    QVBoxLayout *tableLayout = new QVBoxLayout(m_tableContainer);
    tableLayout->setContentsMargins(0, 0, 0, 0);
    tableLayout->setSpacing(0);

    m_tableWidget = new QTableWidget(m_tableContainer);
    m_tableWidget->setColumnCount(4);
    m_tableWidget->setHorizontalHeaderLabels(
        QStringList() << "错误类型" << "时间" << "工位编号" << "日志信息"
    );
    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableWidget->setAlternatingRowColors(true);
    m_tableWidget->setShowGrid(false);
    m_tableWidget->verticalHeader()->setVisible(false);
    m_tableWidget->setStyleSheet(
        "QTableWidget {"
        "    background-color: white;"
        "    border: none;"
        "    gridline-color: transparent;"
        "    font-size: 12px;"
        "}"
        "QTableWidget::item {"
        "    padding: 6px 10px;"
        "    border-bottom: 1px solid #f0f4f8;"
        "}"
        "QTableWidget::item:selected {"
        "    background-color: #e3f2fd;"
        "    color: #1a1a2e;"
        "}"
        "QHeaderView::section {"
        "    background-color: #f8fafc;"
        "    color: #4a5568;"
        "    border: none;"
        "    border-right: 1px solid #e0e6ed;"
        "    border-bottom: 2px solid #e0e6ed;"
        "    padding: 8px 10px;"
        "    font-size: 12px;"
        "    font-weight: bold;"
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

    // 设置列宽
    m_tableWidget->horizontalHeader()->setStretchLastSection(true);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    m_tableWidget->setColumnWidth(0, 80);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    m_tableWidget->setColumnWidth(1, 160);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    m_tableWidget->setColumnWidth(2, 90);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);

    tableLayout->addWidget(m_tableWidget);
    mainLayout->addWidget(m_tableContainer, 1);
}

void LogQuery::refresh()
{
    // 从数据库查询
    QList<int> typeFilter = getActiveTypes();

    QDateTime startTime;
    QDateTime endTime;
    if (m_chkTimeFilter->isChecked()) {
        startTime = m_startTimeEdit->dateTime();
        endTime = m_endTimeEdit->dateTime();
    }

    QString station = m_stationEdit->text().trimmed();

    QVector<DatabaseLogRecord> records =
        Database::instance()->queryLogs(typeFilter, startTime, endTime, station);

    // 填充表格
    m_tableWidget->setRowCount(0);
    m_logs.clear();

    for (const auto &rec : records) {
        LogEntry entry;
        entry.type = rec.type;
        entry.timestamp = rec.timestamp;
        entry.stationNumber = rec.stationNumber;
        entry.message = rec.message;
        m_logs.append(entry);

        int row = m_tableWidget->rowCount();
        m_tableWidget->insertRow(row);

        QString typeText;
        switch (rec.type) {
            case Error:   typeText = "ERROR"; break;
            case Warning: typeText = "警告";  break;
            case Info:    typeText = "信息";  break;
            default:      typeText = "未知";  break;
        }

        m_tableWidget->setItem(row, 0, new QTableWidgetItem(typeText));
        m_tableWidget->setItem(row, 1, new QTableWidgetItem(
            rec.timestamp.toString("yyyy-MM-dd HH:mm:ss")));
        m_tableWidget->setItem(row, 2, new QTableWidgetItem(rec.stationNumber));
        m_tableWidget->setItem(row, 3, new QTableWidgetItem(rec.message));

        updateRowColors(row, rec.type);
    }

    m_tableWidget->scrollToBottom();
}

QList<int> LogQuery::getActiveTypes() const
{
    QList<int> types;
    if (m_errorBtn->isChecked())   types.append(Error);
    if (m_warningBtn->isChecked()) types.append(Warning);
    if (m_infoBtn->isChecked())    types.append(Info);
    return types;
}

void LogQuery::updateRowColors(int row, int type)
{
    QColor textColor;
    switch (type) {
        case Error:
            textColor = QColor(198, 40, 40);
            break;
        case Warning:
            textColor = QColor(245, 127, 23);
            break;
        case Info:
            textColor = QColor(21, 101, 192);
            break;
        default:
            textColor = QColor(74, 85, 104);
            break;
    }

    for (int col = 0; col < 4; ++col) {
        QTableWidgetItem *item = m_tableWidget->item(row, col);
        if (item) {
            item->setForeground(textColor);
        }
    }
}

void LogQuery::applyFilter()
{
    // 本地筛选（对已查询的数据做二次筛选）
    bool showError = m_errorBtn->isChecked();
    bool showWarning = m_warningBtn->isChecked();
    bool showInfo = m_infoBtn->isChecked();
    QString stationFilter = m_stationEdit->text().trimmed();

    for (int i = 0; i < m_logs.size(); ++i) {
        bool visible = true;
        const LogEntry &entry = m_logs[i];

        // 按类型筛选
        if (entry.type == Error && !showError)   visible = false;
        if (entry.type == Warning && !showWarning) visible = false;
        if (entry.type == Info && !showInfo)     visible = false;

        // 按工位编号筛选
        if (!stationFilter.isEmpty()) {
            if (!entry.stationNumber.contains(stationFilter, Qt::CaseInsensitive)) {
                visible = false;
            }
        }

        // 按时间筛选
        if (m_chkTimeFilter->isChecked()) {
            QDateTime startTime = m_startTimeEdit->dateTime();
            QDateTime endTime = m_endTimeEdit->dateTime();
            if (entry.timestamp < startTime || entry.timestamp > endTime) {
                visible = false;
            }
        }

        m_tableWidget->setRowHidden(i, !visible);
    }
}

void LogQuery::onFilterChanged()
{
    applyFilter();
}

void LogQuery::onStationFilterChanged()
{
    applyFilter();
}

void LogQuery::onTimeFilterChanged()
{
    applyFilter();
}

void LogQuery::onQueryClicked()
{
    // 点击"查询"按钮：从数据库重新加载数据
    refresh();
}

void LogQuery::onCollapseClicked()
{
    static bool collapsed = false;
    collapsed = !collapsed;
    if (collapsed) {
        m_tableContainer->hide();
        m_collapseBtn->setText("▶");
    } else {
        m_tableContainer->show();
        m_collapseBtn->setText("▼");
    }
}
