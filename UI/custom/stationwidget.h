#ifndef STATIONWIDGET_H
#define STATIONWIDGET_H

#include <QWidget>
#include <QString>

class StationWidget : public QWidget
{
    Q_OBJECT
public:
    enum Status { OK, NG, Idle };

    explicit StationWidget(QWidget *parent = nullptr);

    // Label1: center of the circle
    void setLabel1(const QString &text);
    QString label1() const { return m_label1; }

    // Per-direction labels (top/bottom/left/right)
    void setTopLabels(const QString &label2, const QString &label3, const QString &label4, const QString &label5);
    void setBottomLabels(const QString &label2, const QString &label3, const QString &label4, const QString &label5);
    void setLeftLabels(const QString &label2, const QString &label3, const QString &label4, const QString &label5);
    void setRightLabels(const QString &label2, const QString &label3, const QString &label4, const QString &label5);

    // Per-direction status
    void setTopStatus(Status status);
    void setBottomStatus(Status status);
    void setLeftStatus(Status status);
    void setRightStatus(Status status);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    struct DirectionData {
        QString label2;
        QString label3;
        QString label4;
        QString label5;
        Status  status;
    };

    static QColor statusColor(Status s);
    void paintDirection(QPainter &painter, const QRectF &rect,
                        const DirectionData &data, bool horizontal);

    QString m_label1;
    DirectionData m_top;
    DirectionData m_bottom;
    DirectionData m_left;
    DirectionData m_right;
};

#endif // STATIONWIDGET_H
