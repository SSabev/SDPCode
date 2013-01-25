#ifndef BTCOMM_H
#define BTCOMM_H

#include <QObject>
#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>

#include "../../Shared/SharedMem.h"

typedef struct{
    unsigned char r_sensor  : 1;
    unsigned char l_sensor  : 1;
    unsigned char have_ball : 1;
} __attribute__ ((packed)) TReadData;


class CBtComm
        : public QObject
{
    Q_OBJECT
public:
    CBtComm();
    ~CBtComm();

    void SendData(TRobotData *data);
    bool ReadData(TReadData &data);

private slots:
    void NewCleintSlot();
    void ClientLost();
    void ClientSockErr();

private:
    QLocalServer m_socketServ;
    QLocalSocket *m_clientSock;
};

#endif // BTCOMM_H
