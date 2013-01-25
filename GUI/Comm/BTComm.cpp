#include "BTComm.h"
#include "../../Shared/Sockets.h"
#include "../../Shared/Logging.h"

CBtComm::CBtComm()
    : m_socketServ(this)
    , m_clientSock(0)
{
    connect(&m_socketServ, SIGNAL(newConnection()), this, SLOT(NewCleintSlot()));
    m_socketServ.listen(BT_COMM_SOCKET_NAME);
}

CBtComm::~CBtComm()
{
    if(m_clientSock != 0)
        m_clientSock->close();

    m_socketServ.close();
}

void CBtComm::NewCleintSlot()
{
    loggingObj->ShowMsg("BTCOMM: client connected");
    m_clientSock = m_socketServ.nextPendingConnection();
    if(m_clientSock == 0) {
        loggingObj->ShowMsg("BTCOMM: failed to get client socket");
        return;
    }

    connect(m_clientSock, SIGNAL(disconnected()), this, SLOT(ClientLost()));
    connect(m_clientSock, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(ClientSockErr()));
}

void CBtComm::ClientLost()
{
    disconnect(m_clientSock);
    m_clientSock->deleteLater();
    m_clientSock = 0;
    loggingObj->ShowMsg("BTCOMM: client closed connection");
}

void CBtComm::ClientSockErr()
{
    loggingObj->ShowMsg(QString("BTCOMM: Client socket error: %1")
                        .arg(m_clientSock->errorString())
                        .toAscii()
                        .data());
}

void CBtComm::SendData(TRobotData *data)
{
    int written;

    if(m_clientSock == 0){
        loggingObj->ShowMsg("BTCOMM: failed to send data - uninitialised socket");
        return;
    }

    written = m_clientSock->write((const char *)data, sizeof(TRobotData));
    if(written != sizeof(TRobotData))
        loggingObj->ShowMsg(QString("BTCOMM: written size differs from expected: expected %1, written %2")
                            .arg(sizeof(TRobotData))
                            .arg(written)
                            .toAscii()
                            .data());
}

bool CBtComm::ReadData(TReadData &data)
{
    int read;

    if(m_clientSock == 0){
        loggingObj->ShowMsg("BTCOMM: failed to read data - uninitialised socket");
        return false;
    }

    read = m_clientSock->read((char *) &data, sizeof(TReadData));
    if(read != sizeof(TReadData)){
        loggingObj->ShowMsg(QString("BTCOMM: read size differs from expected: expected %1, written %2")
                            .arg(sizeof(TReadData))
                            .arg(read)
                            .toAscii()
                            .data());
        return false;
    }

    return true;
}
