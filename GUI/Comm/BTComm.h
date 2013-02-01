#ifndef BTCOMM_H
#define BTCOMM_H

#include <QWidget>
#include <QtNetwork/QTcpSocket>

#include "../../Shared/SharedMem.h"

class CBtComm
        : public QWidget
{
    Q_OBJECT
public:
    CBtComm(QWidget *parent = 0);
    ~CBtComm();

    void SendData(TRobotData *data);
    bool ReadData(TRobotState *data);

    void ConnectToBT();

private slots:
    void ConnectedSlot();
    void ConnLost();
    void SockErr();

private:
    QTcpSocket  m_clientSock;
};

#endif // BTCOMM_H
