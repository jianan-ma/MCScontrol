#ifndef PLCCONFIG_H
#define PLCCONFIG_H

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

class PLCConfig : public QWidget
{
    Q_OBJECT
public:
    explicit PLCConfig(QWidget *parent = nullptr);

    void loadConfig(const QString &iniPath = QString());
    void saveConfig(const QString &iniPath = QString());

private:
    void setupUI();

    QLineEdit *edtIP;
    QLineEdit *edtPLCStation;
    QLineEdit *edtHeartInterval;
    QLineEdit *edtScanInterval;
    QLineEdit *edtNet;
    QLineEdit *edtPCStation;
    QLineEdit *edtGlassID;
    QLineEdit *edtHeartOvertime;
    QLineEdit *edtPCID;
    QLineEdit *edtPath;
    QComboBox *cbo;
    QPushButton *btnLoad;
    QPushButton *btnSave;

    QString m_iniPath;

private slots:
    void onbtnLoadClick();
    void onbtnSaveClick();
};

#endif // PLCCONFIG_H