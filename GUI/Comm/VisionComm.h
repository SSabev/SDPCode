#ifndef VISIONCOMM_H
#define VISIONCOMM_H

#include <QWidget>
#include <QLocalSocket>
#include <QMutex>

#include <SharedMem.h>

#define BUFF_SIZE_MASK  (BUFF_SIZE - 1)
#define BUFF_SIZE       2

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
    void DataReady();

private:
    void ShutdownVision();

    QLocalSocket localSocket;
    QMutex       m_mutex;

    TVisionData  m_visionBuff[BUFF_SIZE];
    unsigned     m_buffIdx;

#ifdef DRY_RUN
    bool m_isConnected;
#endif
};

#endif // VISIONCOMM_H
