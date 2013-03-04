#ifndef CVISIONMOD_H
#define CVISIONMOD_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>

class CVisionMod
        : public QWidget
{
    Q_OBJECT

public:
    CVisionMod (QWidget *parent = 0);

    void UpdateWindow ();

protected:
    //QT virtual functions
    void paintEvent (QPaintEvent *event);
    QSize minimumSizeHint () const;

private:
    void DrawFrame (QPainter *painter);
    QPixmap m_pixmap;
};

#endif // CVISIONMOD_H
