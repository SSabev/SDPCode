#include "BTComm.h"

#include "../../Shared/Sockets.h"
#include "../../Shared/Logging.h"
/*
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <fcntl.h>
*/
#include <QMessageBox>

CBtComm::CBtComm(QWidget *parent)
    : QWidget(parent)
{
/*
    int flags;
    struct sockaddr_in sa;
    struct hostent *server;

    server = gethostbyname("localhost");

    memset(&sa, 0, sizeof(struct sockaddr_in));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(BT_COMM_SOCKET_PORT);
    bcopy( (char *)server->h_addr,
           (char *)&sa.sin_addr.s_addr,
                server->h_length);

    m_socketFd = socket( AF_INET, SOCK_STREAM, 0 );
    flags = fcntl(m_socketFd, F_GETFL, 0);          // get flags
    fcntl(m_socketFd, F_SETFL, flags | O_NONBLOCK); // set non-blocking flag
    ::connect(m_socketFd, (struct sockaddr*) &sa, sizeof(sa));

*/
    connect(&m_clientSock, SIGNAL(connected()), this, SLOT(ConnectedSlot()));
    connect(&m_clientSock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SockErr()));
    connect(&m_clientSock, SIGNAL(disconnected()), this, SLOT(ConnLost()));

    m_clientSock.connectToHost("localhost", BT_COMM_SOCKET_PORT);

}

CBtComm::~CBtComm()
{
    m_clientSock.close();
}

void CBtComm::ConnectedSlot()
{
    loggingObj->ShowMsg("BTCOMM: connected");
}

void CBtComm::ConnLost()
{
    QMessageBox msgBox(this);
    loggingObj->ShowMsg("BTCOMM: connection lost");

    msgBox.setText("Connection to BlueTooth module lost");
    msgBox.setInformativeText("Reconnect to the module?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);

    if(QMessageBox::Yes == msgBox.exec())
        m_clientSock.connectToHost("localhost", BT_COMM_SOCKET_PORT);
}

void CBtComm::ConnectToBT()
{
    if(m_clientSock.state() == QAbstractSocket::ConnectedState)
        return;

    m_clientSock.connectToHost("localhost", BT_COMM_SOCKET_PORT);
}

void CBtComm::SockErr()
{
    loggingObj->ShowMsg(QString("BTCOMM: socket error: %1")
                        .arg(m_clientSock.errorString())
                        .toAscii()
                        .data());
}

void CBtComm::SendData(TRobotData *data)
{
    int written;

    if(m_clientSock.state() != QAbstractSocket::ConnectedState){
        loggingObj->ShowMsg("BTCOMM: failed to send data - not connected");
        return;
    }

    written = m_clientSock.write((const char *)data, sizeof(TRobotData));

    if(written != sizeof(TRobotData))
        loggingObj->ShowMsg(QString("BTCOMM: written size differs from expected: expected %1, written %2")
                            .arg(sizeof(TRobotData))
                            .arg(written)
                            .toAscii()
                            .data());
}

bool CBtComm::ReadData(TRobotState *data)
{
    int read;

    if(m_clientSock.state() != QAbstractSocket::ConnectedState){
        loggingObj->ShowMsg("BTCOMM: failed to read data - not connected");
        return false;
    }

    read = m_clientSock.read((char *) data, sizeof(TRobotState));
    if(read != sizeof(TRobotState)){
        loggingObj->ShowMsg(QString("BTCOMM: read size differs from expected: expected %1, written %2")
                            .arg(sizeof(TRobotState))
                            .arg(read)
                            .toAscii()
                            .data());
        return false;
    }

    return true;
}
