#include "KeysWidget.h"

#include <QKeyEvent>

//#include <stdio.h>

CKeysWidget::CKeysWidget(QWidget *parent)
    : QWidget (parent, Qt::Window)
    , m_state(0)
{
    resize(80, 80);
}

void CKeysWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat()) return;

    switch (event->key()){
    case Qt::Key_Left:
//            printf("Left\n");
            m_state|= (int)eLeft;

        break;
    case Qt::Key_Right:
//            printf("Right\n");
            m_state|= (int)eRight;

        break;
    case Qt::Key_Up:
//            printf("Forward\n");
            m_state|= (int)eUp;
        break;
    case Qt::Key_Down:
//            printf("Backward\n");
            m_state|= (int)eDown;
        break;
    }

    emit Directions(m_state);
}

//enum EDirections{ | Masks
//    eUp    = 1,   | 0xFFFFFFFE
//    eDown  = 2,   | 0xFFFFFFFD
//    eLeft  = 4,   | 0xFFFFFFFB
//    eRight = 8    | 0xFFFFFFF7
//};

void CKeysWidget::keyReleaseEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat()) return;

    switch (event->key()){
    case Qt::Key_Left:
        m_state&= 0xFFFFFFFB;
//        printf("Left released\n");
        break;
    case Qt::Key_Right:
        m_state&= 0xFFFFFFF7;
//        printf("Right released\n");
        break;
    case Qt::Key_Up:
        m_state&= 0xFFFFFFFE;
//        printf("Forward released\n");
        break;
    case Qt::Key_Down:
        m_state&= 0xFFFFFFFD;
//        printf("Backward released\n");
        break;
    }

    emit Directions(m_state);
}
