#ifndef BTCOMM_H
#define BTCOMM_H

#include <QWidget>
#include <QtNetwork/QTcpSocket>

#include "../../Shared/SharedMem.h"

typedef struct{
    unsigned char r_sensor  : 1;
    unsigned char l_sensor  : 1;
    unsigned char have_ball : 1;
} __attribute__ ((packed)) TReadData;


class CBtComm
        : public QWidget
{
    Q_OBJECT
public:
    CBtComm(QWidget *parent = 0);
    ~CBtComm();

    void SendData(TRobotData *data);
    bool ReadData(TReadData &data);

    void ConnectToBT();

private slots:
    void ConnectedSlot();
    void ConnLost();
    void SockErr();

private:
    QTcpSocket  m_clientSock;
//    int                 m_socketFd;
};

#endif // BTCOMM_H
