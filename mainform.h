#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>
#include <QStackedWidget>
#include <QGridLayout>
#include <QMouseEvent>
#include "workstation.h"
#include "stationwidget.h"
#include "realtime_log.h"
#include "clientpg.h"
#include <QThread>
class MainForm : public QWidget
{
    Q_OBJECT
public:
    explicit MainForm(QWidget *parent = nullptr);
    ~MainForm();
    void addWorkStation(WorkStation* station);
    void addWorkStation(StationWidget* station);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    QStackedWidget *stackedwidget;
    QWidget *mainwidget;
    QGridLayout *gridlayout;
    int stationNums = 0;
    bool isMoving=false;
    QPoint startPoint;
    RealTimeLog *realtimeLog;
    //临时添加pg
    QThread *thread_PG;
    ClientPG *clientpg;
signals:
};

#endif // MAINFORM_H
