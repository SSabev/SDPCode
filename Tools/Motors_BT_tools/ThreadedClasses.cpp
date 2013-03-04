#include "ThreadedClasses.h"

#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>

char buff[32];

CConnector::CConnector(int *socket)
    : m_pSocket (socket)
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

//////////////////////////////////////////////////////

CReader::CReader(int *socket)
    : m_pSocket(socket)
{

}

CReader::~CReader()
{

}

void CReader::process()
{
    fd_set readfds;
    fd_set writefds;

    int maxfd;
    int status;

    while(true){
        FD_ZERO (&readfds);
        FD_ZERO (&writefds);
        FD_SET (*m_pSocket, &readfds);

        maxfd = (*m_pSocket) + 1;

        status = select(maxfd, &readfds, &writefds, NULL, NULL);

        if( status > 0 && FD_ISSET( *m_pSocket, &readfds ) ) {
            // incoming data
            memset(&buff, 0, 32);

            status = recv(*m_pSocket, buff, 31, 0);
            if(status < 0){
                // conection lost
                emit ConnectionLost();
                break;
            }
            printf("%s", buff);
        }
        else if(status < 0){
            // error on socket
            emit SocketError();
            break;
        }
    }
}
