#ifndef THREADEDCLASSES_H
#define THREADEDCLASSES_H

#include <QObject>
#include <QString>

#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <sys/socket.h>
#include <sys/select.h>


class CConnector
        : public QObject
{
    Q_OBJECT

public:
    CConnector(struct sockaddr_rc *m_arduinoAddr, int *socket);
    ~CConnector();

public slots:
    void process();

signals:
    void finished(bool isSuccess);

private:
    struct sockaddr_rc  *m_arduinoAddr;
    int                 *m_pSocket;
};

#endif // THREADEDCLASSES_H
