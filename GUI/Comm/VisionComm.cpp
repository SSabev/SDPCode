#include <QMessageBox>

#include <Sockets.h>
#include <Logging.h>
#include <SharedMem.h>

#include "VisionComm.h"

CVisionComm::CVisionComm(QWidget *parent)
    : QWidget(parent)
    , localSocket(this)
{
    connect(&localSocket, SIGNAL(connected()), this, SLOT(ConnedToServ()));
    connect(&localSocket, SIGNAL(error(QLocalSocket::LocalSocketError)),
            this, SLOT(SockErr()));
    connect(&localSocket, SIGNAL(disconnected()), this, SLOT(ConnLost()));

    localSocket.connectToServer(VISION_SOCK_NAME);
}

CVisionComm::~CVisionComm()
{
    ShutdownVision();
}

void CVisionComm::ConnedToServ()
{
    char sendByte;
    int read;

    TPitchCfg cfg;

    loggingObj->ShowMsg("VISIONCOMM: connected");

    // read pitch cfg - first data block received from the Vision module
    sendByte = VISION_REQUEST_CFG;
    localSocket.write(&sendByte, 1);
    localSocket.waitForBytesWritten();
    read = localSocket.read((char *) &cfg, sizeof(TPitchCfg));
    if(read != sizeof(TPitchCfg))
        loggingObj->ShowMsg("VISIONCOMM: failed to read the pitch configuration");
    else
        sharedMem.pitchCfg = cfg;
}

void CVisionComm::ConnectToVision()
{
    if(localSocket.state() == QLocalSocket::ConnectedState)
        return;

    localSocket.connectToServer(VISION_SOCK_NAME);
}

void CVisionComm::ShutdownVision()
{
    char sendByte = VISION_SHUTDOWN;

    if(localSocket.state() != QLocalSocket::ConnectedState)
        return;

    localSocket.write(&sendByte, 1);
    localSocket.flush();
    localSocket.close();
}

bool CVisionComm::IsConnected()
{
    return localSocket.state() == QLocalSocket::ConnectedState;
}

void CVisionComm::ConnLost()
{
    QMessageBox msgBox(this);

    loggingObj->ShowMsg("VISIONCOMM: connection lost");

    msgBox.setText("Connection to Vision module lost");
    msgBox.setInformativeText("Reconnect to the module?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);

    if(QMessageBox::Yes == msgBox.exec())
        localSocket.connectToServer(VISION_SOCK_NAME);
}

void CVisionComm::SockErr()
{
    loggingObj->ShowMsg(QString("VISIONCOMM: socket error: %1")
                        .arg(localSocket.errorString())
                        .toAscii()
                        .data());
}

bool CVisionComm::ReadData(TVisionData *data)
{
    int net;
    char sendByte;

    if(localSocket.state() != QLocalSocket::ConnectedState){
        loggingObj->ShowMsg("VISIONCOMM: failed to read data - not connected");
        return false;
    }

    sendByte = VISION_REQUEST_NAV;
    localSocket.write(&sendByte, 1);
    localSocket.waitForBytesWritten();
    net = localSocket.read((char *) data, sizeof(TVisionData));

    if(net != sizeof(TVisionData)){
        loggingObj->ShowMsg(QString("VISIONCOMM: read size differs from expected: expected %1, read %2")
                            .arg(sizeof(TVisionData))
                            .arg(net)
                            .toAscii()
                            .data());
        return false;
    }
    return true;
}
