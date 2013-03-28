#include "ArduinoComm.h"
#include "ThreadedClasses.h"

#include <Logging.h>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <stdio.h>

#define ARDUINO_MAC "00:12:12:24:71:46"

CArduinoComm::CArduinoComm(QWidget *parent)
    : QWidget(parent)
    , m_status (eDisconnected)
    , m_mutex(QMutex::Recursive)
{
    memcpy(&m_arduinoMAC, ARDUINO_MAC, 18);

    // set the connection parameters (who to connect to)
    m_arduinoAddr.rc_family = AF_BLUETOOTH;
    m_arduinoAddr.rc_channel = 1;
    str2ba(m_arduinoMAC, &m_arduinoAddr.rc_bdaddr);

    memset(&robotState, 0, sizeof(robotState));

    loggingObj->ShowMsg("BT: Ready");
}

CArduinoComm::~CArduinoComm()
{
#ifndef DRY_RUN
    if(m_status != eDisconnected){
        m_thread->terminate();
        ::close(m_Socket);
    }
#endif
}

bool CArduinoComm::ReadData(TRobotState *data)
{
    QMutexLocker locker(&m_mutex);

#ifdef DRY_RUN
    printf("CArduinoComm::ReadData\n");
#else

    if(m_status != eConnected){
        loggingObj->ShowMsg("BT: Can't read data - not connected");
        return false;
    }

    *data = robotState;
#endif
    return true;
}

bool CArduinoComm::IsConnected()
{
    return m_status == eConnected;
}

void CArduinoComm::ConnectToRobot()
{
#ifdef DRY_RUN
    m_status = eConnected;
    loggingObj->ShowMsg("BT: Connected");
#else
    int status;

    CConnector *connector;

    if (m_status != eDisconnected) return;

    // allocate a socket
    m_Socket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // put socket in non-blocking mode
    status = fcntl ( m_Socket, F_GETFL, 0 );
    fcntl ( m_Socket, F_SETFL, status | O_NONBLOCK );

    // initiate connection attempt
    status = ::connect (m_Socket, (struct sockaddr *) &m_arduinoAddr, sizeof (m_arduinoAddr));
    if (0 != status && errno != EINPROGRESS ) {
        loggingObj->ShowMsg("BT: Failed to initialize connection");
        return;
    }

    m_status = eConnecting;

    m_thread = new QThread;
    connector = new CConnector (&m_Socket);
    connector->moveToThread (m_thread);

    connect (connector, SIGNAL (finished (bool)), this, SLOT (ConnResult (bool)));
    connect (connector, SIGNAL (finished (bool)), m_thread, SLOT (quit ()));
    connect (connector, SIGNAL (finished (bool)), connector, SLOT (deleteLater ()));

    connect (m_thread, SIGNAL (started ()), connector, SLOT (process ()));
    connect (m_thread, SIGNAL (finished ()), m_thread, SLOT (deleteLater ()));
    connect (m_thread, SIGNAL (terminated ()), m_thread, SLOT (deleteLater ()));

    m_thread->start();
#endif
}

void CArduinoComm::ConnResult(bool isConnected)
{
    CReader *reader;

    if (!isConnected){
        loggingObj->ShowMsg("BT: Failed to connect to Arduino");
        m_status = eDisconnected;
        return;
    }

    m_status = eConnected;
    loggingObj->ShowMsg("BT: Connected");
    sharedMem.systemStatus  = (TSystemStatus)((int)sharedMem.systemStatus | (int) eBTConnected);

    // Now need to start listening thread
    m_thread = new QThread;
    reader = new CReader(&m_Socket, &robotState);
    reader->moveToThread(m_thread);

    connect(reader, SIGNAL(ConnectionLost()), this, SLOT(ConnLost()));
    connect(reader, SIGNAL(ReadMissMatch(int)), this, SLOT(ReadMissMatch(int)));
    connect(reader, SIGNAL(SocketError()), this, SLOT(SocketError()));

    connect(reader, SIGNAL(ConnectionLost()), m_thread, SLOT(quit()));
    connect(reader, SIGNAL(ConnectionLost()), reader, SLOT(deleteLater()));

    connect(reader, SIGNAL(SocketError()), m_thread, SLOT(quit()));
    connect(reader, SIGNAL(SocketError()), reader, SLOT(deleteLater()));

    connect(m_thread, SIGNAL(started()), reader, SLOT(process()));
    connect(m_thread, SIGNAL(finished()), m_thread, SLOT(deleteLater()));
    connect(m_thread, SIGNAL(terminated()), m_thread, SLOT(deleteLater()));

    m_thread->start();
}

void CArduinoComm::ConnLost()
{
    loggingObj->ShowMsg("BT: Connection lost");
    ::close(m_Socket);

    m_status = eDisconnected;
    sharedMem.systemStatus  = (TSystemStatus)((int)sharedMem.systemStatus & (int) eBTDisconnected);
}

void CArduinoComm::ReadMissMatch(int read)
{
    QString str("BT: Read Mismatch: read %1, expected %2");
    loggingObj->ShowMsg(str.arg(read).arg(sizeof(TRobotState)).toAscii().data());
}

void CArduinoComm::SocketError()
{
    loggingObj->ShowMsg("BT: Socket error");
    ::close(m_Socket);

    m_status = eDisconnected;
    sharedMem.systemStatus  = (TSystemStatus)((int)sharedMem.systemStatus & (int) eBTDisconnected);
}

bool CArduinoComm::SendData(TRobotData *data)
{
    int status;
    QString str;

    QMutexLocker locker(&m_mutex);

#ifdef DRY_RUN
    printf("CArduinoComm::SendData\n");
#else


    if(m_status != eConnected){
        loggingObj->ShowMsg("BT: Can't send data - not connected");
        return false;
    }

    status = ::send(m_Socket, (void *)data, sizeof(TRobotData), 0);
    if(status != sizeof(TRobotData)){
        if(status == -1){
            // Error -> connection lost (?)
            loggingObj->ShowMsg("BT: Failed to send data; Closing connection");
            m_thread->terminate();
            ::close(m_Socket);

            m_status = eDisconnected;
            sharedMem.systemStatus  = (TSystemStatus)((int)sharedMem.systemStatus & (int) eBTDisconnected);
        }
        else{
            str = "BT: Sending data mismatch: sent %1, sending %2";
            loggingObj->ShowMsg(str.arg(status).arg( sizeof(TRobotData)).toAscii().data());
        }
        return false;
    }
    else
#endif
        return true;

}
