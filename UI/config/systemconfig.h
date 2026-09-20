#ifndef SYSTEMCONFIG_H
#define SYSTEMCONFIG_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSettings>

class SystemConfig : public QWidget
{
    Q_OBJECT
public:
    explicit SystemConfig(QWidget *parent = nullptr);

    void loadConfig(const QString &iniPath = QString());
    void saveConfig(const QString &iniPath = QString());

private:
    void setupUI();
    void applyStyle();

    // 系统设置
    QComboBox *m_cboLanguage;
    QComboBox *m_cboTheme;
    QLineEdit *m_edtSystemName;
    QCheckBox *m_chkAutoStart;
    QCheckBox *m_chkAutoLogin;

    // 路径设置
    QLineEdit *m_edtDataPath;
    QLineEdit *m_edtLogPath;
    QLineEdit *m_edtRecipePath;
    QLineEdit *m_edtReportPath;

    // 数据库设置
    QLineEdit *m_edtDbHost;
    QLineEdit *m_edtDbPort;
    QLineEdit *m_edtDbName;
    QLineEdit *m_edtDbUser;
    QLineEdit *m_edtDbPassword;

    // 运行参数
    QLineEdit *m_edtHeartbeatInterval;
    QLineEdit *m_edtScanInterval;
    QLineEdit *m_edtLogKeepDays;
    QCheckBox *m_chkSaveLog;

    // 配置文件
    QLineEdit *m_edtPath;
    QPushButton *m_btnLoad;
    QPushButton *m_btnSave;

    QString m_iniPath;

private slots:
    void onBtnLoadClick();
    void onBtnSaveClick();
};

#endif // SYSTEMCONFIG_H