#ifndef KEYSWIDGET_H
#define KEYSWIDGET_H

#include <QWidget>

class CKeysWidget
        : public QWidget
{
    Q_OBJECT

public:
    CKeysWidget(QWidget *parent = 0);

    enum EDirections{
        eUp    = 1,
        eDown  = 2,
        eLeft  = 4,
        eRight = 8
    };

signals:
    void Directions(int dir);

protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

private:
    int m_state;
};

#endif // KEYSWIDGET_H
