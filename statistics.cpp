#include "statistics.h"
#include <QHeaderView>
#include <QSizePolicy>

// ============================================================================
// 前向声明 Database 单例的 StationResult 查询接口（用户后续实现）
// 若项目中已有 Database 类，请删除此临时定义并改为 #include "database.h"
// ============================================================================

struct StationResult {
    QString stationName;
    int     totalCount;
    int     passCount;
    int     failCount;
    double  passRate;
};

// 临时 Database 占位类
class DatabaseForStats
{
public:
    static DatabaseForStats *instance()
    {
        static DatabaseForStats instance;
        return &instance;
    }

    QVector<StationResult> queryStationResults(
        const QDateTime &startTime,
        const QDateTime &endTime,
        const QString   &stationFilter)
    {
        Q_UNUSED(startTime)
        Q_UNUSED(endTime)
        Q_UNUSED(stationFilter)
        // TODO: 用户实现数据库查询
        return QVector<StationResult>();
    }
};

// ============================================================================

Statistics::Statistics(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void Statistics::setupUI()
{
    setStyleSheet(
        "Statistics {"
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
//    connect(m_collapseBtn, &QPushButton::clicked, this, &Statistics::onCollapseClicked);

    // 标题
    m_titleLabel = new QLabel("各站点结果统计", m_headerWidget);
    m_titleLabel->setStyleSheet(
        "color: #1a1a2e;"
        "font-size: 14px;"
        "font-weight: bold;"
    );

   // headerLayout->addWidget(m_collapseBtn);
    headerLayout->addWidget(m_titleLabel);
    headerLayout->addSpacing(12);

    // 时间筛选复选框
    m_chkTimeFilter = new QCheckBox("时间", m_headerWidget);
    m_chkTimeFilter->setStyleSheet(
        "QCheckBox {"
        "    color: #4a5568;"
        "    font-size: 12px;"
        "    spacing: 4px;"
        "}"
    );
    m_chkTimeFilter->setChecked(true);
    headerLayout->addWidget(m_chkTimeFilter);

    // 起始时间
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

    QLabel *timeSep = new QLabel("~", m_headerWidget);
    timeSep->setStyleSheet("color: #4a5568; font-size: 12px;");

    // 结束时间
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

    connect(m_chkTimeFilter, &QCheckBox::toggled, this, &Statistics::onTimeFilterToggled);

    headerLayout->addWidget(m_startTimeEdit);
    headerLayout->addWidget(timeSep);
    headerLayout->addWidget(m_endTimeEdit);

    // 站点筛选
    m_stationEdit = new QLineEdit(m_headerWidget);
    m_stationEdit->setPlaceholderText("站点名称(空=全部)");
    m_stationEdit->setFixedHeight(28);
    m_stationEdit->setFixedWidth(140);
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
            this, &Statistics::onStationFilterChanged);

    headerLayout->addWidget(m_stationEdit);

    // 查询按钮
    m_queryBtn = new QPushButton("查询", m_headerWidget);
    m_queryBtn->setFixedSize(60, 28);
    m_queryBtn->setCursor(Qt::PointingHandCursor);
    m_queryBtn->setStyleSheet(
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
    connect(m_queryBtn, &QPushButton::clicked, this, &Statistics::onQueryClicked);

    headerLayout->addWidget(m_queryBtn);
    headerLayout->addStretch();

    mainLayout->addWidget(m_headerWidget);

    // === 统计表格 ===
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
    m_tableWidget->setColumnCount(5);
    m_tableWidget->setHorizontalHeaderLabels(
        QStringList() << "站点名称" << "测试总数" << "合格数" << "不合格数" << "合格率"
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
        "    padding: 8px 10px;"
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
        "    padding: 10px;"
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

    // 列宽
    m_tableWidget->horizontalHeader()->setStretchLastSection(true);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    m_tableWidget->setColumnWidth(1, 100);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    m_tableWidget->setColumnWidth(2, 100);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    m_tableWidget->setColumnWidth(3, 100);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
    m_tableWidget->setColumnWidth(4, 110);

    // 对齐方式：数字类列右对齐或居中
    m_tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    tableLayout->addWidget(m_tableWidget);
    mainLayout->addWidget(m_tableContainer, 1);
}

void Statistics::refresh()
{
    QDateTime startTime;
    QDateTime endTime;
    if (m_chkTimeFilter->isChecked()) {
        startTime = m_startTimeEdit->dateTime();
        endTime = m_endTimeEdit->dateTime();
    }
    QString station = m_stationEdit->text().trimmed();

    QVector<StationResult> results =
        DatabaseForStats::instance()->queryStationResults(startTime, endTime, station);

    m_stats.clear();
    for (const auto &r : results) {
        StationStat s;
        s.stationName = r.stationName;
        s.totalCount = r.totalCount;
        s.passCount = r.passCount;
        s.failCount = r.failCount;
        s.passRate = r.passRate;
        m_stats.append(s);
    }

    populateTable(m_stats);
}

void Statistics::populateTable(const QVector<StationStat> &stats)
{
    m_tableWidget->setRowCount(0);

    int totalSum = 0, passSum = 0, failSum = 0;

    for (const auto &s : stats) {
        int row = m_tableWidget->rowCount();
        m_tableWidget->insertRow(row);

        QTableWidgetItem *nameItem = new QTableWidgetItem(s.stationName);
        nameItem->setForeground(QColor("#1a1a2e"));
        nameItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        m_tableWidget->setItem(row, 0, nameItem);

        QTableWidgetItem *totalItem = new QTableWidgetItem(QString::number(s.totalCount));
        totalItem->setTextAlignment(Qt::AlignCenter);
        m_tableWidget->setItem(row, 1, totalItem);

        QTableWidgetItem *passItem = new QTableWidgetItem(QString::number(s.passCount));
        passItem->setTextAlignment(Qt::AlignCenter);
        passItem->setForeground(QColor("#2e7d32"));
        m_tableWidget->setItem(row, 2, passItem);

        QTableWidgetItem *failItem = new QTableWidgetItem(QString::number(s.failCount));
        failItem->setTextAlignment(Qt::AlignCenter);
        failItem->setForeground(QColor("#c62828"));
        m_tableWidget->setItem(row, 3, failItem);

        QTableWidgetItem *rateItem = new QTableWidgetItem(
            QString::number(s.passRate, 'f', 2) + "%");
        rateItem->setTextAlignment(Qt::AlignCenter);
        m_tableWidget->setItem(row, 4, rateItem);
        updatePassRateCellStyle(row, 4, s.passRate);

        totalSum += s.totalCount;
        passSum += s.passCount;
        failSum += s.failCount;
    }

    if (stats.size() > 1) {
        addSummaryRow(totalSum, passSum, failSum);
    } else if (stats.size() == 0) {
        // 空数据占位
        int row = m_tableWidget->rowCount();
        m_tableWidget->insertRow(row);
        QTableWidgetItem *emptyItem = new QTableWidgetItem("暂无数据");
        emptyItem->setTextAlignment(Qt::AlignCenter);
        emptyItem->setForeground(QColor("#9aa5b1"));
        m_tableWidget->setSpan(row, 0, 1, 5);
        m_tableWidget->setItem(row, 0, emptyItem);
    }
}

void Statistics::addSummaryRow(int totalSum, int passSum, int failSum)
{
    int row = m_tableWidget->rowCount();
    m_tableWidget->insertRow(row);

    double overallRate = (totalSum == 0) ? 0.0 : (double)passSum * 100.0 / totalSum;

    QTableWidgetItem *nameItem = new QTableWidgetItem("汇总");
    nameItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    nameItem->setFont(QFont("", 12, QFont::Bold));
    nameItem->setBackground(QColor("#f0f4f8"));
    nameItem->setForeground(QColor("#1a1a2e"));
    m_tableWidget->setItem(row, 0, nameItem);

    QTableWidgetItem *totalItem = new QTableWidgetItem(QString::number(totalSum));
    totalItem->setTextAlignment(Qt::AlignCenter);
    totalItem->setFont(QFont("", 12, QFont::Bold));
    totalItem->setBackground(QColor("#f0f4f8"));
    m_tableWidget->setItem(row, 1, totalItem);

    QTableWidgetItem *passItem = new QTableWidgetItem(QString::number(passSum));
    passItem->setTextAlignment(Qt::AlignCenter);
    passItem->setFont(QFont("", 12, QFont::Bold));
    passItem->setForeground(QColor("#2e7d32"));
    passItem->setBackground(QColor("#f0f4f8"));
    m_tableWidget->setItem(row, 2, passItem);

    QTableWidgetItem *failItem = new QTableWidgetItem(QString::number(failSum));
    failItem->setTextAlignment(Qt::AlignCenter);
    failItem->setFont(QFont("", 12, QFont::Bold));
    failItem->setForeground(QColor("#c62828"));
    failItem->setBackground(QColor("#f0f4f8"));
    m_tableWidget->setItem(row, 3, failItem);

    QTableWidgetItem *rateItem = new QTableWidgetItem(
        QString::number(overallRate, 'f', 2) + "%");
    rateItem->setTextAlignment(Qt::AlignCenter);
    rateItem->setFont(QFont("", 12, QFont::Bold));
    rateItem->setBackground(QColor("#f0f4f8"));
    m_tableWidget->setItem(row, 4, rateItem);
    updatePassRateCellStyle(row, 4, overallRate);
}

void Statistics::updatePassRateCellStyle(int row, int col, double passRate)
{
    QTableWidgetItem *item = m_tableWidget->item(row, col);
    if (!item) return;

    QColor fg;
    if (passRate >= 98.0) {
        fg = QColor("#2e7d32");   // 深绿 - 优秀
    } else if (passRate >= 95.0) {
        fg = QColor("#558b2f");   // 绿 - 良好
    } else if (passRate >= 90.0) {
        fg = QColor("#f57f17");   // 橙 - 一般
    } else {
        fg = QColor("#c62828");   // 红 - 较差
    }
    item->setForeground(fg);
}

void Statistics::onTimeFilterToggled(bool checked)
{
    m_startTimeEdit->setEnabled(checked);
    m_endTimeEdit->setEnabled(checked);
}

void Statistics::onStationFilterChanged()
{
    refresh();
}

void Statistics::onQueryClicked()
{
    refresh();
}

void Statistics::onCollapseClicked()
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
