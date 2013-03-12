#include "tcp_server.h"
#include <iostream>
#include <sstream>
#include <winsock.h>
using namespace std;

SOCKET socket_client; //Socket handle

//CONNECTTOHOST – Connects to a remote host
bool ConnectToHost(int PortNo, char* IPAddress)
{
    cout << "Player 2 - Client - TCP Connecting to Server on port 9000\n";

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
    target.sin_addr.s_addr = inet_addr("127.0.0.1");; //Target IP

    socket_client = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); //Create socket
    if (socket_client == INVALID_SOCKET)
    {
        return false; //Couldn't create the socket
    }

    //Try connecting...

    if (connect(socket_client, (SOCKADDR *)&target, sizeof(target)) == SOCKET_ERROR)
    {
        cout << "Client connect failure\n";
        return false; //Couldn't connect
    }
    else
        cout << "Client connect success\n";
        return true; //Success
}

server_data ClientRead(void)
{
  server_data d = { };
  int nret;;
  nret = recv(socket_client, (char*)&d, sizeof(d),0);
  cout<<"Server: ["<< d.s_x << "," << d.s_y << "]" << " Ball: [" <<  d.b_x << "," << d.b_y << "," << d.b_z << "]\n";
  return d;
}

void ClientWrite(float x, float y)
{
  int nret;
  client_data d = { x, y };
  nret = send(socket_client, (char*)&d, sizeof(d),0);
}

//CLOSECONNECTION – shuts down the socket and closes any connection on it
void CloseConnection ()
{
    //Close the socket if it exists
    if (socket_client)
        closesocket(socket_client);

    WSACleanup(); //Clean up Winsock
}