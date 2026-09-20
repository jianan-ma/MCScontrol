#include "stationwidget.h"
#include <QPainter>
#include <QPainterPath>

StationWidget::StationWidget(QWidget *parent)
    : QWidget(parent),
      m_label1(QStringLiteral("Label1")),
      m_top{QStringLiteral("Label2"), QStringLiteral("Label3"), QStringLiteral("Label4"), QStringLiteral("Label5"), OK},
      m_bottom{QStringLiteral("Label2"), QStringLiteral("Label3"), QStringLiteral("Label4"), QStringLiteral("Label5"), NG},
      m_left{QStringLiteral("Label2"), QStringLiteral("Label3"), QStringLiteral("Label4"), QStringLiteral("Label5"), Idle},
      m_right{QStringLiteral("Label2"), QStringLiteral("Label3"), QStringLiteral("Label4"), QStringLiteral("Label5"), Idle}
{
    setMinimumSize(260, 260);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

QColor StationWidget::statusColor(Status s)
{
    switch (s) {
    case OK:   return QColor(76, 175, 80);    // green
    case NG:   return QColor(244, 67, 54);    // red
    case Idle: return QColor(158, 158, 158);  // gray
    }
    return QColor(158, 158, 158);
}

// --- Public setters ---

void StationWidget::setLabel1(const QString &text)
{
    if (m_label1 != text) { m_label1 = text; update(); }
}

void StationWidget::setTopLabels(const QString &l2, const QString &l3, const QString &l4, const QString &l5)
{
    m_top.label2 = l2; m_top.label3 = l3; m_top.label4 = l4; m_top.label5 = l5; update();
}

void StationWidget::setBottomLabels(const QString &l2, const QString &l3, const QString &l4, const QString &l5)
{
    m_bottom.label2 = l2; m_bottom.label3 = l3; m_bottom.label4 = l4; m_bottom.label5 = l5; update();
}

void StationWidget::setLeftLabels(const QString &l2, const QString &l3, const QString &l4, const QString &l5)
{
    m_left.label2 = l2; m_left.label3 = l3; m_left.label4 = l4; m_left.label5 = l5; update();
}

void StationWidget::setRightLabels(const QString &l2, const QString &l3, const QString &l4, const QString &l5)
{
    m_right.label2 = l2; m_right.label3 = l3; m_right.label4 = l4; m_right.label5 = l5; update();
}

void StationWidget::setTopStatus(Status s)    { m_top.status    = s; update(); }
void StationWidget::setBottomStatus(Status s) { m_bottom.status = s; update(); }
void StationWidget::setLeftStatus(Status s)   { m_left.status   = s; update(); }
void StationWidget::setRightStatus(Status s)  { m_right.status  = s; update(); }

// --- Painting ---

void StationWidget::paintDirection(QPainter &painter, const QRectF &rect,
                                   const DirectionData &data, bool horizontal)
{
    const qreal radius = 10.0;
    const qreal dotR   = 16.0;    // status dot radius
    const qreal dotPad = 6.0;    // padding inside rect for dot
    const qreal textH  = 14.0;   // line height for each label
    const qreal footH  = textH + 4.0; // footer area height

    painter.save();

    // --- Rounded rectangle background ---
    QPainterPath path;
    path.addRoundedRect(rect, radius, radius);
    painter.fillPath(path, QColor(240, 244, 248));
    painter.setPen(QPen(QColor(200, 210, 220), 1.0));
    painter.drawPath(path);

    // --- Status dot (top-left corner of rect) ---
    QPointF dotCenter;
    if (horizontal) {
        // horizontal rects (left / right): dot at top-left
        dotCenter = QPointF(rect.left() + dotPad + dotR, rect.top() + dotPad + dotR);
    } else {
        // vertical rects (top / bottom): dot at top-left
        dotCenter = QPointF(rect.left() + dotPad + dotR, rect.top() + dotPad + dotR);
    }
    painter.setPen(Qt::NoPen);
    painter.setBrush(statusColor(data.status));
    painter.drawEllipse(dotCenter, dotR, dotR);

    // --- Content area (to the right of the dot, above the footer) ---
    qreal contentLeft = dotCenter.x() + dotR + 6.0;
    qreal contentTop  = rect.top() + dotPad;
    qreal contentRight = rect.right() - dotPad;
    qreal contentBottom = rect.bottom() - footH;
    QRectF contentArea(contentLeft, contentTop,
                       contentRight - contentLeft, contentBottom - contentTop);

    if (contentArea.width() > 0 && contentArea.height() > 0) {
        QFont labelFont;
        labelFont.setPixelSize(qBound(10, (int)(contentArea.height() / 3.5), 18));
        painter.setFont(labelFont);
        painter.setPen(QColor(60, 70, 80));

        qreal lineY = contentArea.top() + textH;
        painter.drawText(QRectF(contentArea.left(), lineY - textH, contentArea.width(), textH),
                         Qt::AlignCenter, data.label2);
        lineY += textH*1.5;
        painter.drawText(QRectF(contentArea.left(), lineY - textH, contentArea.width(), textH),
                         Qt::AlignCenter, data.label3);
        lineY += textH*1.5;
        painter.drawText(QRectF(contentArea.left(), lineY - textH, contentArea.width(), textH),
                         Qt::AlignCenter, data.label4);
    }

    // --- Footer label (Label5) at bottom of rect ---
    QFont footFont;
    footFont.setPixelSize(qBound(12, (int)(footH * 0.75), 14));
    painter.setFont(footFont);
    painter.setPen(QColor(100, 110, 120));
    QRectF footRect(rect.left() + dotPad, rect.bottom() - footH + 2,
                    rect.width() - 2 * dotPad, footH - 4);
    painter.drawText(footRect, Qt::AlignHCenter | Qt::AlignVCenter, data.label5);

    painter.restore();
}

void StationWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const int w = width();
    const int h = height();

    // --- Layout geometry ---
    qreal circleD = qMin(w, h)*0.8;
    qreal circleR = circleD / 2.0;
    QPointF center(w / 2.0, h / 2.0);

    // Rect dimensions
    qreal BotW = circleD*0.5;
    qreal BotH = circleD * 0.35;

    // Position rects so they slightly overlap the circle
    qreal gap = circleR * 0.12;

    QRectF topRect(center.x() - BotW / 2.0,10,BotW, BotH);

    QRectF bottomRect(center.x() - BotW / 2.0,h-BotH-10,BotW, BotH);

    QRectF leftRect(10,center.y() - BotH / 2.0,BotW, BotH);

    QRectF rightRect(w -10 - BotW,center.y() - BotH / 2.0,BotW, BotH);

    // --- Draw bottom-layer circle with Label1 ---
    painter.setPen(QPen(QColor(180, 200, 220), 2.0));
    painter.setBrush(QColor(230, 240, 250));
    painter.drawEllipse(center, circleR, circleR);

    QFont centerFont;
    centerFont.setPixelSize(qBound(12, (int)(circleD * 0.20), 24));
    centerFont.setBold(true);
    painter.setFont(centerFont);
    painter.setPen(QColor(40, 50, 60));
    QRectF textRect(center.x() - circleR, center.y() - circleR,
                    circleD, circleD);
    painter.drawText(textRect, Qt::AlignCenter, m_label1);

    // --- Draw the four rounded rectangles on top ---
    // Order: left, right, top, bottom — to ensure consistent overlap
    paintDirection(painter, leftRect,   m_left,   false);
    paintDirection(painter, rightRect,  m_right,  false);
    paintDirection(painter, topRect,    m_top,    true);
    paintDirection(painter, bottomRect, m_bottom, true);
}
