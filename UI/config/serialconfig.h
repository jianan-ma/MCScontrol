#ifndef SERIALCONFIG_H
#define SERIALCONFIG_H

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSettings>

class SerialConfig : public QWidget
{
    Q_OBJECT
public:
    explicit SerialConfig(QWidget *parent = nullptr);

    void loadConfig(const QString &iniPath = QString());
    void saveConfig(const QString &iniPath = QString());

    QString portName() const;
    int baudRate() const;
    int dataBits() const;
    QString parity() const;
    int stopBits() const;
    QString flowControl() const;

private:
    void setupUI();
    void initControls();

    QComboBox *m_cboPortName;
    QComboBox *m_cboBaudRate;
    QComboBox *m_cboDataBits;
    QComboBox *m_cboParity;
    QComboBox *m_cboStopBits;
    QComboBox *m_cboFlowControl;

    QLineEdit *m_edtPath;
    QPushButton *m_btnLoad;
    QPushButton *m_btnSave;

    QString m_iniPath;

private slots:
    void onBtnLoadClick();
    void onBtnSaveClick();
};

#endif // SERIALCONFIG_H