#ifndef REALTIME_LOG_H
#define REALTIME_LOG_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMap>
#include <QVector>

class RealTimeLog : public QWidget
{
    Q_OBJECT
public:
    enum LogType {
        Error,
        Warning,
        Info
    };

    explicit RealTimeLog(QWidget *parent = nullptr);

    void addLog(LogType type, const QString &timestamp,
                const QString &stationNumber, const QString &message);
    void clearLogs();

private slots:
    void onFilterChanged();
    void onStationFilterChanged();
    void onCollapseClicked();

private:
    struct LogEntry {
        LogType type;
        QString timestamp;
        QString stationNumber;
        QString message;
    };

    void setupUI();
    void applyFilter();
    void updateRowColors(int row, LogType type);

    QPushButton *m_collapseBtn;
    QLabel *m_titleLabel;
    QPushButton *m_errorBtn;
    QPushButton *m_warningBtn;
    QPushButton *m_infoBtn;
    QLineEdit *m_stationEdit;
    QPushButton *m_searchBtn;
    QTableWidget *m_tableWidget;

    QVector<LogEntry> m_logs;
    bool m_collapsed;

    QWidget *m_headerWidget;
    QWidget *m_tableContainer;
};

#endif // REALTIME_LOG_H
