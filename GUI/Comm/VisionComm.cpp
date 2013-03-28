#include <QMessageBox>

#include <Sockets.h>
#include <Logging.h>
#include <SharedMem.h>

#include "VisionComm.h"

CVisionComm::CVisionComm(QWidget *parent)
    : QWidget(parent)
    , localSocket(this)
    , m_mutex(QMutex::Recursive)
    , m_buffIdx(0)
{
#ifdef DRY_RUN
    m_isConnected = false;
#else
    connect(&localSocket, SIGNAL(connected()), this, SLOT(ConnedToServ()));
    connect(&localSocket, SIGNAL(error(QLocalSocket::LocalSocketError)),
            this, SLOT(SockErr()));
    connect(&localSocket, SIGNAL(disconnected()), this, SLOT(ConnLost()));

    memset(&m_visionBuff, 0xFF, sizeof(m_visionBuff)); // this makes it invalid according to AI

    localSocket.connectToServer(VISION_SOCK_NAME);
#endif
}

CVisionComm::~CVisionComm()
{
#ifndef DRY_RUN
    disconnect(&localSocket);
//    ShutdownVision();
#endif
}

void CVisionComm::ConnedToServ()
{
    int read;

    TPitchCfg cfg;

    loggingObj->ShowMsg("VISIONCOMM: connected");

    // read pitch cfg - first data block received from the Vision module
    localSocket.waitForReadyRead();
    read = localSocket.read((char *) &cfg, sizeof(TPitchCfg));
    if(read != sizeof(TPitchCfg))
        loggingObj->ShowMsg("VISIONCOMM: failed to read the pitch configuration");
    else
        sharedMem.pitchCfg = cfg;

    connect(&localSocket, SIGNAL(readyRead()), this, SLOT(DataReady()));
}

void CVisionComm::ConnectToVision()
{
#ifdef DRY_RUN
    loggingObj->ShowMsg("VISIONCOMM: connected");
    m_isConnected = true;
#else

    if(localSocket.state() == QLocalSocket::ConnectedState)
        return;

    localSocket.connectToServer(VISION_SOCK_NAME);
#endif
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
#ifdef DRY_RUN
    return m_isConnected;
#else
    return localSocket.state() == QLocalSocket::ConnectedState;
#endif
}

void CVisionComm::ConnLost()
{
    disconnect(&localSocket, SIGNAL(readyRead()), this, SLOT(DataReady()));

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

void CVisionComm::DataReady()
{
    unsigned nextIdx;
    int net;
    TVisionData data;

    net = localSocket.read((char *) &data, sizeof(TVisionData));

    if(net != sizeof(TVisionData)){
        loggingObj->ShowMsg(QString("VISIONCOMM: read size differs from expected: expected %1, read %2")
                            .arg(sizeof(TVisionData))
                            .arg(net)
                            .toAscii()
                            .data());
    }
    else{
        nextIdx = (m_buffIdx + 1)&BUFF_SIZE_MASK;
        m_visionBuff[nextIdx] = data;

        m_mutex.lock();
        m_buffIdx = nextIdx;
        m_mutex.unlock();
    }
}

bool CVisionComm::ReadData(TVisionData *data)
{

#ifdef DRY_RUN
    printf("CVisionComm::ReadData\n");
#else

    if(localSocket.state() != QLocalSocket::ConnectedState){
        loggingObj->ShowMsg("VISIONCOMM: failed to read data - not connected");
        return false;
    }

    m_mutex.lock();
    *data = m_visionBuff[m_buffIdx];
    m_mutex.unlock();

#endif
    return true;
}
