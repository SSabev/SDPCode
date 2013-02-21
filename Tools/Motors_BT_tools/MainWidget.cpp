#include "MainWidget.h"
#include "ThreadedClasses.h"

//#include "../../Shared/SharedMem.h"

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <QMessageBox>
#include <QApplication>

#define ARDUINO_MAC "00:12:12:24:71:46"

typedef struct{
    //! TODO: need to identify data that is required
    ///       for robot movement
    /// For testing (Milestone 1) it's size is 2 integers
    unsigned char motor_left;
    unsigned char motor_right;
    unsigned char motor_front;
    unsigned char motor_rear;
    unsigned char kicker;
} __attribute__ ((packed)) TRobotData;

TRobotData _robotData;

CMainWidget::CMainWidget (QWidget *parent)
    : QWidget (parent)
    , m_status (eDisconnected)
    , keysWdgt(this)
{
    setupUi (this);
    sendValsBtn->setEnabled(false);

    connect (connectBtn,    SIGNAL (clicked()), this, SLOT (BT_Connect()));
    connect (disconnectBtn, SIGNAL (clicked()), this, SLOT (BT_Disconnect()));
    connect (sendValsBtn,   SIGNAL (clicked()),  this, SLOT (SendVals()));
    connect (exitBtn,       SIGNAL (clicked()), this, SLOT (ExitSlot()));

    connect (&keysWdgt,     SIGNAL(Directions(int)), this, SLOT(MoveInDir(int)));

    memcpy (&m_arduinoMAC, ARDUINO_MAC, 18);

    // set the connection parameters (who to connect to)
    m_arduinoAddr.rc_family = AF_BLUETOOTH;
    m_arduinoAddr.rc_channel = 1;
    str2ba (m_arduinoMAC, &m_arduinoAddr.rc_bdaddr);

    memset (&_robotData, 0, sizeof (_robotData));

    keysWdgt.show();
}

void CMainWidget::BT_Connect()
{
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
        QMessageBox::critical (this,
                               "Connecting to Arduino",
                               "Failed to initialize connection",
                               QMessageBox::Ok);
        return;
    }

    statLabel->setText ("Connecting");
    m_status = eConnecting;

    m_thread = new QThread;
    connector = new CConnector (&m_arduinoAddr, &m_Socket);
    connector->moveToThread (m_thread);

    connect (connector, SIGNAL (finished (bool)), this, SLOT (ConnResult (bool)));
    connect (connector, SIGNAL (finished (bool)), m_thread, SLOT (quit ()));
    connect (connector, SIGNAL (finished (bool)), connector, SLOT (deleteLater ()));

    connect (m_thread, SIGNAL (started ()), connector, SLOT (process ()));
    connect (m_thread, SIGNAL (finished ()), m_thread, SLOT (deleteLater ()));
    connect (m_thread, SIGNAL (terminated ()), m_thread, SLOT (deleteLater ()));

    m_thread->start();
}

void CMainWidget::ConnResult(bool isConnected)
{
    if (!isConnected){
        statLabel->setText("Connection failed");
        m_status = eDisconnected;
        return;
    }

    statLabel->setText("Connected");
    sendValsBtn->setEnabled(true);

    m_status = eConnected;
}

void CMainWidget::BT_Disconnect()
{
    if (m_status == eDisconnected) return;

    else if (m_status == eConnecting) {
        m_thread->terminate();
    }

    ::close(m_Socket);
    m_status = eDisconnected;
    statLabel->setText ("Disconnected");
    sendValsBtn->setEnabled(false);
}

void CMainWidget::SendVals()
{
    int status;
    if (m_status != eConnected) return;

    _robotData.motor_left = (unsigned char) leftBox->value();
    _robotData.motor_right = (unsigned char) rightBox->value();
    _robotData.motor_rear = (unsigned char) rearBox->value();
    _robotData.motor_front = (unsigned char) frontBox->value();

    status = ::send (m_Socket, (void *) &_robotData, sizeof(_robotData), 0);

    if (status != sizeof(_robotData)) {
         QMessageBox::warning(this,
                              "Sending Values",
                              QString("Sending: %1 bytes, actually sent: %2 bytes")
                                .arg(sizeof(_robotData))
                                .arg(status),
                              QMessageBox::Ok);
    }
}

void CMainWidget::ExitSlot()
{
    BT_Disconnect();
    qApp->exit();
}

void CMainWidget::MoveInDir(int dir)
{
    int status;
    if (m_status != eConnected) return;

    if(dir & CKeysWidget::eLeft)
        _robotData.motor_left = (unsigned char) leftBox->value();
    else
        _robotData.motor_left = 0;

    if(dir & CKeysWidget::eRight)
         _robotData.motor_right = (unsigned char) rightBox->value();
    else
        _robotData.motor_right = 0;

    if(dir & CKeysWidget::eUp)
        _robotData.motor_front = (unsigned char) frontBox->value();
    else
        _robotData.motor_front = 0;

    if(dir & CKeysWidget::eDown)
        _robotData.motor_rear = (unsigned char) rearBox->value();
    else
        _robotData.motor_rear = 0;

//    printf("Front: %d\nRear: %d\nLeft: %d\nRight: %d\n\n\n",
//           (int)_robotData.motor_front,
//           (int)_robotData.motor_rear,
//           (int)_robotData.motor_left,
//           (int)_robotData.motor_right);

    status = ::send (m_Socket, (void *) &_robotData, sizeof(_robotData), 0);

    if (status != sizeof(_robotData)) {
         QMessageBox::warning(this,
                              "Sending Values",
                              QString("Sending: %1 bytes, actually sent: %2 bytes")
                                .arg(sizeof(_robotData))
                                .arg(status),
                              QMessageBox::Ok);
    }
}
