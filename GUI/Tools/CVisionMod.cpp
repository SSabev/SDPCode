#include "CVisionMod.h"

#include <QStylePainter>

#include <SharedMem.h>

#include <cmath>

#define H_SIZE  640
#define V_SIZE  480

#define POINT_THICKNESS  8
#define PATH_THICKNESS   3

#define BALL_THICKNESS      10

#define ORIENTATION_LINE_LEN    10
#define ORIENTATION_THICKNESS   4

#define POSITION_THICKNESS      10

CVisionMod::CVisionMod (QWidget *parent)
    : QWidget (parent, Qt::Window)
    , m_pitchWidth(H_SIZE)
    , m_pitchHeight(V_SIZE)
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
    return QSize (m_pitchWidth, m_pitchHeight);
}

void CVisionMod::SetSize(int w, int h)
{
    m_pitchWidth = w;
    m_pitchHeight = h;

    resize(m_pitchWidth, m_pitchHeight);
}

void CVisionMod::paintEvent(QPaintEvent* /*event*/)
{
    QStylePainter painter(this);
    painter.drawPixmap(0, 0, m_pixmap);
}

void CVisionMod::DrawFrame (QPainter *painter)
{
    int i;

    QVector<QPointF> orientations;

    TEntry *entry = &sharedMem.positioning[sharedMem.currentIdx];
    QBrush brushPen (sharedMem.teamColor == eBlueTeam ? QColor(0x00, 0x34, 0xff) : QColor(0xff, 0xC9, 0x00),
                     Qt::SolidPattern);
    QPen pen (brushPen, POSITION_THICKNESS, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPainterPath path;
    qreal dist;

    if (entry->aiData.pathLength == 0) return;  // nothing to do here

    // paint BG
    painter->fillRect (0, 0, m_pitchWidth, m_pitchHeight, Qt::white);

    // plot current position (from AI)
    painter->setPen (pen);
    // we are guaranteed that there is at least 1 point - current position
    painter->drawPoint((int)entry->aiData.path[0].position_X, m_pitchHeight - (int)entry->aiData.path[0].position_Y);
    path.moveTo(entry->aiData.path[0].position_X, m_pitchHeight - (int)entry->aiData.path[0].position_Y);

    orientations.append(QPointF(entry->aiData.path[0].position_X,
                        m_pitchHeight - entry->aiData.path[0].position_Y));
    orientations.append(QPointF(entry->aiData.path[0].position_X + ORIENTATION_LINE_LEN*cos(entry->aiData.path[0].orientation),
                        m_pitchHeight - entry->aiData.path[0].position_Y - ORIENTATION_LINE_LEN*sin(entry->aiData.path[0].orientation)));

    // plot the ball
    if((entry->visionData.ball_x > 0) && (entry->visionData.ball_x < m_pitchWidth) &&
            (entry->visionData.ball_y > 0) && (entry->visionData.ball_y < m_pitchHeight)){
        pen.setColor (Qt::red);
        pen.setWidth (BALL_THICKNESS);
        painter->setPen (pen);
        painter->drawPoint(entry->visionData.ball_x, m_pitchHeight - entry->visionData.ball_y);
    }

    // plot the opponent position
    if(sharedMem.teamColor == eBlueTeam){
        if((entry->visionData.yellow_x > 0) && (entry->visionData.yellow_x < m_pitchWidth) &&
                (entry->visionData.yellow_y > 0) && (entry->visionData.yellow_y < m_pitchHeight)){
            pen.setColor (QColor(0xff, 0xC9, 0x00));
            pen.setWidth (POSITION_THICKNESS);
            painter->setPen (pen);

            painter->drawPoint(entry->visionData.yellow_x, m_pitchHeight - entry->visionData.yellow_y);

            orientations.append(QPointF(entry->visionData.yellow_x,
                                        m_pitchHeight - entry->visionData.yellow_y));
            orientations.append(QPointF(entry->visionData.yellow_x + ORIENTATION_LINE_LEN*cos(entry->visionData.yellow_angle),
                                        m_pitchHeight - entry->visionData.yellow_y - ORIENTATION_LINE_LEN*sin(entry->visionData.yellow_angle)));

        }
    }
    else{
        if((entry->visionData.blue_x > 0) && (entry->visionData.blue_x < m_pitchWidth) &&
                (entry->visionData.blue_y > 0) && (entry->visionData.blue_y < m_pitchHeight)){
            pen.setColor (QColor(0x00, 0x34, 0xff));
            pen.setWidth (POSITION_THICKNESS);
            painter->setPen (pen);

            painter->drawPoint(entry->visionData.blue_x, m_pitchHeight - entry->visionData.blue_y);

            orientations.append(QPointF(entry->visionData.blue_x,
                                        m_pitchHeight - entry->visionData.blue_y));
            orientations.append(QPointF(entry->visionData.blue_x + ORIENTATION_LINE_LEN*cos(entry->visionData.blue_angle),
                                        m_pitchHeight - entry->visionData.blue_y - ORIENTATION_LINE_LEN*sin(entry->visionData.blue_angle)));

        }
    }


    pen.setColor (QColor(0xFF, 0x00, 0xEA));
    painter->setPen (pen);
    for (i = 1; i < (int) entry->aiData.pathLength; i++){
        painter->drawPoint((int)entry->aiData.path[i].position_X, m_pitchHeight - (int)entry->aiData.path[i].position_Y);

        dist = entry->aiData.path[i].position_X - entry->aiData.path[i-1].position_X;
        path.cubicTo(entry->aiData.path[i-1].position_X + dist/2,
                     m_pitchHeight - entry->aiData.path[i-1].position_Y,
                     entry->aiData.path[i-1].position_X + dist/2,
                     m_pitchHeight - entry->aiData.path[i].position_Y,
                     entry->aiData.path[i].position_X,
                     m_pitchHeight - entry->aiData.path[i].position_Y);

        orientations.append(QPointF(
                                entry->aiData.path[i].position_X,
                                m_pitchHeight - entry->aiData.path[i].position_Y
                                )
                            );
        orientations.append(QPointF(
                                entry->aiData.path[i].position_X + ORIENTATION_LINE_LEN*cos(entry->aiData.path[i].orientation),
                                m_pitchHeight - entry->aiData.path[i].position_Y - ORIENTATION_LINE_LEN*sin(entry->aiData.path[i].orientation)
                                )
                            );
    }

    if (i == 1) return;

    pen.setStyle (Qt::DotLine);
    pen.setWidth (PATH_THICKNESS);
    pen.setColor (Qt::green);
    painter->setPen (pen);
    // paint the ideal path :)
    painter->drawPath(path);

    pen.setStyle (Qt::SolidLine);
    pen.setWidth (ORIENTATION_THICKNESS);
    pen.setColor (Qt::black);
    painter->setPen (pen);
    // paint the orientations
    painter->drawLines(orientations);
}
