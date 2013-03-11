#include "tcp_comm.h"
using namespace std;

SOCKET s;
WSADATA w;

int ListenOnPort(int portno)
{
    int error = WSAStartup (0x0202, &w);

    if (error)
    {
        cout << error;
        return false; //For some reason we couldn't start Winsock
    }


    SOCKADDR_IN addr; // The address structure for a TCP socket

    addr.sin_family = AF_INET;      // Address family
    addr.sin_port = htons (portno);   // Assign port to this socket
    addr.sin_addr.s_addr = htonl (INADDR_ANY);

    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Create socket

    if (s == INVALID_SOCKET)
    {
      cout << "error 1\n";
        return false; //Don't continue if we couldn't create a //socket!!
    }

    if (bind(s, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
      cout << "error 2\n";
        return false;
    }

    listen(s, SOMAXCONN);

    //Don't forget to clean up with CloseConnection()!
}