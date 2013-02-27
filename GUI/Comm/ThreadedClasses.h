#ifndef THREADEDCLASSES_H
#define THREADEDCLASSES_H

#include <QObject>
#include <QString>

#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <sys/socket.h>
#include <sys/select.h>

#include <SharedMem.h>


class CConnector
        : public QObject
{
    Q_OBJECT

public:
    CConnector(int *socket);
    ~CConnector();

public slots:
    void process();

signals:
    void finished(bool isSuccess);

private:
    int *m_pSocket;
};




class CReader
        : public QObject
{
    Q_OBJECT

public:
    CReader(int *socket, TRobotState *data);
    ~CReader();

public slots:
    void process();

signals:
    void ConnectionLost();
    void SocketError();
    void ReadMissMatch(int read); // we know what is expected

private:
    int *m_pSocket;
    TRobotState *readData;
};

#endif // THREADEDCLASSES_H
