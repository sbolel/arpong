#include <iostream>
#include <winsock.h>
using namespace std;

SOCKET socket_client; //Socket handle

//CONNECTTOHOST – Connects to a remote host
bool ConnectToHost(int PortNo, char* IPAddress)
{
    //Start up Winsock…
    WSADATA wsadata;

    int error = WSAStartup(0x0202, &wsadata);

    //Did something happen?
    if (error)
        return false;

    //Did we get the right Winsock version?
    if (wsadata.wVersion != 0x0202)
    {
        WSACleanup(); //Clean up Winsock
        return false;
    }

    //Fill out the information needed to initialize a socket…
    SOCKADDR_IN target; //Socket address information

    target.sin_family = AF_INET; // address family Internet
    target.sin_port = htons (PortNo); //Port to connect on
    target.sin_addr.s_addr = inet_addr (IPAddress); //Target IP

    socket_client = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); //Create socket
    if (socket_client == INVALID_SOCKET)
    {
        return false; //Couldn't create the socket
    }

    //Try connecting...

    if (connect(socket_client, (SOCKADDR *)&target, sizeof(target)) == SOCKET_ERROR)
    {
        return false; //Couldn't connect
    }
    else
        return true; //Success
}

void ClientRead(void)
{
  char client_buffer[5];
  // client_buffer[0]: server X pos
  // client_buffer[1]: server Y pos
  // client_buffer[2]: ball X pos
  // client_buffer[3]: ball Y pos
  // client_buffer[4]: ball Z pos
  memset(client_buffer, 0, sizeof(client_buffer)); //Clear the client_buffer
  recv (socket_client, client_buffer, sizeof(client_buffer)-1, 0);
  cout << client_buffer[0] << client_buffer[1] << client_buffer[2] << client_buffer[3] << client_buffer[4];
}

//CLOSECONNECTION – shuts down the socket and closes any connection on it
void CloseConnection ()
{
    //Close the socket if it exists
    if (socket_client)
        closesocket(socket_client);

    WSACleanup(); //Clean up Winsock
}