#ifndef SOCKETCONFIG_H
#define SOCKETCONFIG_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSettings>

class SocketConfig : public QWidget
{
    Q_OBJECT
public:
    explicit SocketConfig(QWidget *parent = nullptr);

    void loadConfig(const QString &iniPath = QString());
    void saveConfig(const QString &iniPath = QString());

private:
    void setupUI();

    QComboBox *m_cboType;
    QLineEdit *m_edtLocalIP;
    QLineEdit *m_edtLocalPort;
    QLineEdit *m_edtRemoteIP;
    QLineEdit *m_edtRemotePort;
    QLineEdit *m_edtConnectTimeout;
    QLineEdit *m_edtReadTimeout;
    QLineEdit *m_edtReconnectInterval;
    QLineEdit *m_edtPath;
    QPushButton *m_btnLoad;
    QPushButton *m_btnSave;

    QString m_iniPath;

private slots:
    void onBtnLoadClick();
    void onBtnSaveClick();
};

#endif // SOCKETCONFIG_H