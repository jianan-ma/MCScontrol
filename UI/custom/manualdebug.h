#ifndef MANUALDEBUG_H
#define MANUALDEBUG_H

#include <QWidget>
#include <QTabWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QSplitter>
#include <QScrollArea>
#include <QFrame>
#include <QMap>
#include <QButtonGroup>
#include <QTextEdit>
class ManualDebug : public QWidget
{
    Q_OBJECT
public:
    enum SystemStatus {
        StatusNormal = 0,
        StatusWarning = 1,
        StatusAlarm = 2
    };
    Q_ENUM(SystemStatus)

    explicit ManualDebug(QWidget *parent = nullptr);

    void setSignalActive(const QString &signalCode, bool active);
    void setSystemStatus(SystemStatus status);
public slots:
    void PgSuccess();
    void PgammaSuccess();
    void LightUp();
    void PgConnect(bool ok);
private:
    void setupUI();
    QWidget *createSidebar();
    QWidget *createPlcSignalTab();
    QWidget *createPgTab();
    QGroupBox *createSignalGroup(const QString &title,
                                  const QStringList &psignals,
                                  int columns);

    // Sidebar
    QPushButton *m_btnDebugMode;
    QRadioButton *m_rdoNormal;
    QRadioButton *m_rdoWarning;
    QRadioButton *m_rdoAlarm;
    QButtonGroup *m_statusGroup;
    QPushButton *m_btnAlarmList;
    QPushButton *m_btnClearAlarm;
    QLineEdit *m_edtAccount;
    QPushButton *m_btnLogin;
    QTreeWidget *m_treeStations;
    QTreeWidget *m_treeClients;

    // Main tabs
    QTabWidget *m_tabWidget;

    // Signal maps for dynamic update
    QMap<QString, QCheckBox*> m_signals;
    QCheckBox *chk;
    QTextEdit *textedit;
signals:
    void sig_PgInit(bool);
    void sig_PgPgamma();
};

#endif // MANUALDEBUG_H
