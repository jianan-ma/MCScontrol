#ifndef STATISTICS_H
#define STATISTICS_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QDateTimeEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QVector>
#include <QDateTime>

// ============================================================================
// 数据库单例接口声明（需由用户后续实现，与 logquery.h 中的 Database 为同一单例）
// ============================================================================
// 假设存在 Database 单例，新增以下接口：
//
//   struct StationResult {
//       QString stationName;    // 站点名称，如 "上料"、"GAMMA"、"AOI"、"工站1"
//       int     totalCount;     // 总测试数量
//       int     passCount;      // 合格数量
//       int     failCount;      // 不合格数量
//       double  passRate;       // 合格率 (0~100)
//   };
//
//   QVector<StationResult> queryStationResults(
//       const QDateTime &startTime,    // 起始时间
//       const QDateTime &endTime,      // 结束时间
//       const QString   &stationFilter // 工位筛选，空=全部
//   );
//
// ============================================================================

class Statistics : public QWidget
{
    Q_OBJECT
public:
    explicit Statistics(QWidget *parent = nullptr);

    // 按当前条件从数据库刷新统计数据
    void refresh();

private slots:
    void onTimeFilterToggled(bool checked);
    void onStationFilterChanged();
    void onQueryClicked();
    void onCollapseClicked();

private:
    struct StationStat {
        QString stationName;
        int     totalCount;
        int     passCount;
        int     failCount;
        double  passRate;
    };

    void setupUI();
    void populateTable(const QVector<StationStat> &stats);
    void addSummaryRow(int totalSum, int passSum, int failSum);
    void updatePassRateCellStyle(int row, int col, double passRate);

    // UI
    QPushButton     *m_collapseBtn;
    QLabel          *m_titleLabel;

    QCheckBox       *m_chkTimeFilter;
    QDateTimeEdit   *m_startTimeEdit;
    QDateTimeEdit   *m_endTimeEdit;
    QLineEdit       *m_stationEdit;
    QPushButton     *m_queryBtn;

    QTableWidget    *m_tableWidget;

    QWidget         *m_headerWidget;
    QWidget         *m_tableContainer;

    // 当前数据
    QVector<StationStat> m_stats;
};

#endif // STATISTICS_H
