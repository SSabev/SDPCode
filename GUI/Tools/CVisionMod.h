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

    void SetSize(int w, int h);

protected:
    //QT virtual functions
    void paintEvent (QPaintEvent*);
    QSize minimumSizeHint () const;

private:
    void DrawFrame (QPainter *painter);
    QPixmap m_pixmap;

    int m_pitchWidth;
    int m_pitchHeight;
};

#endif // CVISIONMOD_H
