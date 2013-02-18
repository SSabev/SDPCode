#include "CVisionMod.h"

#include <QStylePainter>

#include <SharedMem.h>

#define H_SIZE  640
#define V_SIZE  480

#define POINT_THICKNESS  8
#define PATH_THICKNESS   3

CVisionMod::CVisionMod (QWidget *parent)
    : QWidget (parent, Qt::Window)
{
//    setSizePolicy(QSizePolicy::Fixed);
    resize(H_SIZE, V_SIZE);
    this->setFixedSize(this->width(),this->height());

    m_pixmap = QPixmap (size());
    m_pixmap.fill (this, 0, 0);
    QPainter painter (&m_pixmap);
    painter.initFrom (this);
    painter.fillRect (0, 0, H_SIZE, V_SIZE, Qt::white);
}

void CVisionMod::UpdateWindow()
{
    m_pixmap = QPixmap (size());
    m_pixmap.fill (this, 0, 0);
    QPainter painter (&m_pixmap);
    painter.initFrom (this);

    DrawFrame (&painter);

    update();
}

QSize CVisionMod::minimumSizeHint () const
{
    return QSize (H_SIZE, V_SIZE);
}

void CVisionMod::paintEvent(QPaintEvent *event)
{
    QStylePainter painter(this);
    painter.drawPixmap(0, 0, m_pixmap);
}

void CVisionMod::DrawFrame (QPainter *painter)
{
    int i;

    TEntry *entry = &sharedMem.positioning[sharedMem.currentIdx];
    QBrush brushPen (Qt::blue, Qt::SolidPattern);
    QPen pen (brushPen, POINT_THICKNESS, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPainterPath path;
    qreal dist;

    if (entry->aiData.pathLength == 0) return;  // nothing to do here

    // paint BG
    painter->fillRect (0, 0, H_SIZE, V_SIZE, Qt::white);
    // plot current points from AI
    painter->setPen (pen);
    // we are guaranteed that there is at least 1 point - current position
    // draw it in blue
    painter->drawPoint((int)entry->aiData.path[0].position_X, V_SIZE - (int)entry->aiData.path[0].position_Y);
    path.moveTo(entry->aiData.path[0].position_X, V_SIZE - (int)entry->aiData.path[0].position_Y);

    pen.setColor (Qt::red);
    painter->setPen (pen);
    for (i = 1; i < (int) entry->aiData.pathLength; i++){
        painter->drawPoint((int)entry->aiData.path[i].position_X, V_SIZE - (int)entry->aiData.path[i].position_Y);

        dist = entry->aiData.path[i].position_X - entry->aiData.path[i-1].position_X;
        path.cubicTo(entry->aiData.path[i-1].position_X + dist/2,
                     V_SIZE - entry->aiData.path[i-1].position_Y,
                     entry->aiData.path[i-1].position_X + dist/2,
                     V_SIZE - entry->aiData.path[i].position_Y,
                     entry->aiData.path[i].position_X,
                     V_SIZE - entry->aiData.path[i].position_Y);
    }

    if (i == 1) return;

    pen.setStyle (Qt::DotLine);
    pen.setWidth (PATH_THICKNESS);
    pen.setColor (Qt::green);
    painter->setPen (pen);

    painter->drawPath(path);
}
