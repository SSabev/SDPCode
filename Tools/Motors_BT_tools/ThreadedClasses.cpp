#include "ThreadedClasses.h"

#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>

CConnector::CConnector(sockaddr_rc *m_pArduinoAddr, int *socket)
    : m_arduinoAddr (m_pArduinoAddr)
    , m_pSocket (socket)
{

}

CConnector::~CConnector()
{

}

void CConnector::process()
{
    fd_set readfds;
    fd_set writefds;
    int maxfd;
    int status;

    // wait for connection to complete or fail
    FD_ZERO (&readfds);
    FD_ZERO (&writefds);
    FD_SET (*m_pSocket, &writefds);

    maxfd = (*m_pSocket);

    status = select (maxfd + 1, &readfds, &writefds, NULL, NULL);

    if (status > 0 && FD_ISSET (*m_pSocket, &writefds))
        emit finished (true);

    else
        emit finished (false);
}
