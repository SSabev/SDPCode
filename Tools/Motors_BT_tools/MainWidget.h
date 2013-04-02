#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QThread>

#include <sys/socket.h>
#include <sys/select.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include "ui_MainWidget.h"

#include "KeysWidget.h"

class CMainWidget
        : public QWidget
        , public Ui::MainWidget

{
    Q_OBJECT

public:
    CMainWidget (QWidget *parent = 0);

    enum EStatus {
        eDisconnected,
        eConnecting,
        eConnected
    };

signals:
    void ExitSignal();

private slots:
    void SendVals ();
    void SendZero ();
    void BT_Disconnect ();
    void BT_Connect ();
    void ExitSlot ();
    void KickSlot ();

    void ConnResult(bool isConnected);

    void MoveInDir(int dir);

    void ConnLost();
    void SocketError();

private:
    EStatus m_status;

    int m_Socket;
    char m_arduinoMAC[18];// = "00:12:12:24:71:46";
    struct sockaddr_rc m_arduinoAddr;

    fd_set m_readFDs;
    fd_set m_writeFDs;
    int m_maxFD;

    QThread* m_thread;

    CKeysWidget keysWdgt;
};

#endif // MAINWIDGET_H
