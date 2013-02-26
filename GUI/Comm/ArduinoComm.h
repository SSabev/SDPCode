#ifndef ARDUINOCOMM_H
#define ARDUINOCOMM_H

#include <QWidget>
#include <QThread>

#include <SharedMem.h>

#include <sys/socket.h>
#include <sys/select.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include "IBTComm.h"

class CArduinoComm
        : public QWidget
        , IBTComm
{
    Q_OBJECT

public:
    CArduinoComm(QWidget *parent = 0);
    ~CArduinoComm();

    virtual bool SendData(TRobotData *data);
    virtual bool ReadData(TRobotState *data);

    virtual void ConnectToRobot();

private slots:
    void ConnResult(bool isConnected);
    void ConnLost();
    void SocketError();
    void ReadMissMatch(int read); // we know what is expected

private:
    enum EStatus {
        eDisconnected,
        eConnecting,
        eConnected
    };

    EStatus m_status;

    int m_Socket;
    char m_arduinoMAC[18];// = "00:12:12:24:71:46";
    struct sockaddr_rc m_arduinoAddr;

    fd_set m_readFDs;
    fd_set m_writeFDs;
    int m_maxFD;

    QThread* m_thread;

    TRobotState robotState;
};

#endif // ARDUINOCOMM_H
