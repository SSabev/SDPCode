#include "BTComm.h"

#include <Sockets.h>
#include <Logging.h>

#include <QMessageBox>

CBtComm::CBtComm(QWidget *parent)
    : QWidget(parent)
{
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

void CBtComm::ConnectToRobot()
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

bool CBtComm::SendData(TRobotData *data)
{
    int written;

    if(m_clientSock.state() != QAbstractSocket::ConnectedState){
        loggingObj->ShowMsg("BTCOMM: failed to send data - not connected");
        return false;
    }

    written = m_clientSock.write((const char *)data, sizeof(TRobotData));

    if(written != sizeof(TRobotData)){
        loggingObj->ShowMsg(QString("BTCOMM: written size differs from expected: expected %1, written %2")
                            .arg(sizeof(TRobotData))
                            .arg(written)
                            .toAscii()
                            .data());
        return false;
    }
    else
        return true;
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
