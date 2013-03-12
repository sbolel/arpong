#include "tcp_server.h"
#include <iostream>
#include <sstream>
#include <winsock.h>
using namespace std;

SOCKET socket_server;
SOCKET theClient;
WSADATA w;

int ListenOnPort(int portno) {
	cout << "Player 1 - Server - TCP Listening on port 9000\n";
	int nret;
	int error = WSAStartup (0x0202, &w);
	if (error) {
		cout << error;
		return false; //For some reason we couldn't start Winsock
	}

	SOCKADDR_IN addr; // The address structure for a TCP socket
	addr.sin_family = AF_INET; // Address family
	addr.sin_port = htons (portno); // Assign port to this socket
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	socket_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Create socket

	if (socket_server == INVALID_SOCKET) {
		cout << "TCP Error: Invalid Socket\n";
		WSACleanup();
		return false; //Don't continue if we couldn't create a //socket!!
	}

	if (bind(socket_server, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR) {
		cout << "TCP Error: Bind unsuccessful\n";
		return false;
	}

	nret = listen(socket_server, 1);

	if (nret == SOCKET_ERROR) {
		nret = WSAGetLastError();
		WSACleanup();
	}

	theClient = accept(socket_server,NULL,NULL);

	if (theClient == INVALID_SOCKET) {
		nret = WSAGetLastError();
		WSACleanup();
	}
	else {
		cout << "Player 2 Connected to Server";
	}
	//Don't forget to clean up with CloseConnection()!
	return true;
}


client_data ServerRead(void) {
	client_data d = { };
	int nret;
	nret = recv(theClient, (char*)&d, sizeof(d), 0);
	return d;
}

void ServerWrite(glm::vec2 paddle, glm::vec3 ball) {
	int nret;
	server_data d = { paddle, ball };
	nret = send(theClient, (char*)&d, sizeof(d),0);
}