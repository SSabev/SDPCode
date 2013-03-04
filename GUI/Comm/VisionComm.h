#ifndef VISIONCOMM_H
#define VISIONCOMM_H

#include <QWidget>
#include <QLocalSocket>

#include <SharedMem.h>

class CVisionComm
        : public QWidget
{
    Q_OBJECT
public:
    CVisionComm(QWidget *parent = 0);
    ~CVisionComm();

    bool ReadData(TVisionData *data);

    void ConnectToVision();
    void ShutdownVision();
    bool IsConnected();

private slots:
    void ConnedToServ();
    void ConnLost();
    void SockErr();

private:
    QLocalSocket localSocket;
};

#endif // VISIONCOMM_H
