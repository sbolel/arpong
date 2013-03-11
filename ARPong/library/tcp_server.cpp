#include <iostream>
#include <winsock.h>
using namespace std;

SOCKET socket_server;
WSADATA w;

int ListenOnPort(int portno)
{
    int error = WSAStartup (0x0202, &w);
    if (error) {
        cout << error;
        return false; //For some reason we couldn't start Winsock
    }

    SOCKADDR_IN addr; // The address structure for a TCP socket
    addr.sin_family = AF_INET;      // Address family
    addr.sin_port = htons (portno);   // Assign port to this socket
    addr.sin_addr.s_addr = htonl (INADDR_ANY);
    socket_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Create socket

    if (socket_server == INVALID_SOCKET) {
      cout << "TCP Error: Invalid Socket\n";
        return false; //Don't continue if we couldn't create a //socket!!
    }

    if (bind(socket_server, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR) {
      cout << "TCP Error: Bind unsuccessful\n";
        return false;
    }

    listen(socket_server, SOMAXCONN);

    return 0;
    //Don't forget to clean up with CloseConnection()!
}