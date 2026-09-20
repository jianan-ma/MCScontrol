#ifndef MAINCONFIG_H
#define MAINCONFIG_H

#include <QWidget>
#include <QTabWidget>
class MainConfig : public QWidget
{
    Q_OBJECT
public:
    explicit MainConfig(QWidget *parent = nullptr);
private:
    QTabWidget *tabwidget;
signals:

};

#endif // MAINCONFIG_H
