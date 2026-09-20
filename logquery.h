#ifndef LOGQUERY_H
#define LOGQUERY_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QDateTimeEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QVector>
#include <QDateTime>
#include <QCheckBox>
// ============================================================================
// 数据库单例接口声明（由用户后续实现）
// ============================================================================
// 假设存在一个 Database 单例类，提供以下接口：
//
//   class Database : public QObject
//   {
//       Q_OBJECT
//   public:
//       static Database *instance();
//
//       struct LogRecord {
//           int         id;             // 主键
//           int         type;           // 0=Error, 1=Warning, 2=Info
//           QDateTime   timestamp;      // 时间
//           QString     stationNumber;  // 工位编号
//           QString     message;        // 日志信息
//       };
//
//       // 按条件查询日志
//       //   typeFilter:    为空列表表示不限类型
//       //   startTime:     为空 QDateTime 表示不限开始时间
//       //   endTime:       为空 QDateTime 表示不限结束时间
//       //   stationNumber: 为空字符串表示不限工位编号
//       QVector<LogRecord> queryLogs(
//           const QList<int>    &typeFilter,
//           const QDateTime     &startTime,
//           const QDateTime     &endTime,
//           const QString       &stationNumber);
//
//       // 获取日志总数
//       int getLogCount();
//   };
//
// 用户后续实现 Database::instance() 和 queryLogs() 即可。
// ============================================================================

class LogQuery : public QWidget
{
    Q_OBJECT
public:
    enum LogType {
        Error = 0,
        Warning = 1,
        Info = 2
    };

    explicit LogQuery(QWidget *parent = nullptr);

    // 从数据库重新加载日志（按当前筛选条件查询）
    void refresh();

private slots:
    void onFilterChanged();
    void onStationFilterChanged();
    void onTimeFilterChanged();
    void onCollapseClicked();
    void onQueryClicked();

private:
    struct LogEntry {
        int         type;
        QDateTime   timestamp;
        QString     stationNumber;
        QString     message;
    };

    void setupUI();
    void applyFilter();
    void updateRowColors(int row, int type);
    QList<int> getActiveTypes() const;

    // UI
    QPushButton     *m_collapseBtn;
    QLabel          *m_titleLabel;
    QPushButton     *m_errorBtn;
    QPushButton     *m_warningBtn;
    QPushButton     *m_infoBtn;
    QDateTimeEdit   *m_startTimeEdit;
    QDateTimeEdit   *m_endTimeEdit;
    QCheckBox       *m_chkTimeFilter;
    QLineEdit       *m_stationEdit;
    QPushButton     *m_searchBtn;
    QTableWidget    *m_tableWidget;

    QWidget         *m_headerWidget;
    QWidget         *m_tableContainer;

    // 当前查询结果
    QVector<LogEntry> m_logs;
};

#endif // LOGQUERY_H
