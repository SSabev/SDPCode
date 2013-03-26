#ifndef VISIONCOMM_H
#define VISIONCOMM_H

#include <QWidget>
#include <QLocalSocket>
#include <QMutex>

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

    bool IsConnected();

private slots:
    void ConnedToServ();
    void ConnLost();
    void SockErr();

private:
    void ShutdownVision();

    QLocalSocket localSocket;
    QMutex      m_mutex;

#ifdef DRY_RUN
    bool m_isConnected;
#endif
};

#endif // VISIONCOMM_H
