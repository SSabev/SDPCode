#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

int main(int argc, char **argv)
{
    struct sockaddr_rc addr = { 0 };
    int s, status, bytes_read;
    char dest[18] = "00:12:12:24:71:46";
    fd_set readfds, writefds;
    int maxfd, sock_flags;

    char buff[128];
    memset(&buff, 0, 128);

    char lamp = '0';
    int cnt;

    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = 1;
    str2ba( dest, &addr.rc_bdaddr );

    // put socket in non-blocking mode
    sock_flags = fcntl( s, F_GETFL, 0 );
    fcntl( s, F_SETFL, sock_flags | O_NONBLOCK );

    // initiate connection attempt
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    if( 0 != status && errno != EINPROGRESS ) {
        perror("connect asd");
        printf ("err %d\n", status);
        return 1;
    }

    // wait for connection to complete or fail
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_SET(s, &writefds);
//    FD_SET(s, &readfds);
    maxfd = s;

    status = select(maxfd + 1, &readfds, &writefds, NULL, NULL);

    if( status > 0 && FD_ISSET( s, &writefds ) ) {
        printf ("connected\n");
    }
    else if( status < 0 ) perror("operation failed");

    status = send(s, "1", 1, 0);
    if( status < 0 ) perror("uh oh");

    cnt = 0;

    while (true) {
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_SET(s, &readfds);
//        FD_SET(client, &writefds); //Remote device can accept data - it always does

        maxfd = s;

        status = select(maxfd + 1, &readfds, &writefds, NULL, NULL);

        if( status > 0 && FD_ISSET( s, &readfds ) ) {
            // incoming data
            bytes_read = recv(s, buff, sizeof(buff), 0);
            if( 0 == bytes_read ) break;
            printf("received [%s]\n", buff);
        }

        if (cnt == 7){
            cnt = 0;
            send( s, (char *)&lamp, 1, 0 );
            if (lamp == '1') lamp = '0';
            else             lamp = '1';
        }
        else cnt++;
    }

    printf ("Received 0 bytes: host closed connection\n");

    close(s);
    return 0;
}
