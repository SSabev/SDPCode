#ifndef BTCOMM_H
#define BTCOMM_H

#include <QWidget>
#include <QtNetwork/QTcpSocket>

#include <SharedMem.h>

#include "IBTComm.h"

class CBtComm
        : public QWidget
        , IBTComm
{
    Q_OBJECT
public:
    CBtComm(QWidget *parent = 0);
    ~CBtComm();

    virtual bool SendData(TRobotData *data);
    virtual bool ReadData(TRobotState *data);

    virtual void ConnectToRobot();

private slots:
    void ConnectedSlot();
    void ConnLost();
    void SockErr();

private:
    QTcpSocket  m_clientSock;
};

#endif // BTCOMM_H
