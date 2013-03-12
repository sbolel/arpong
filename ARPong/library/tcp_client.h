#ifndef TCP_CLIENT_H_
#define TCP_CLIENT_H_


bool ConnectToHost(int PortNo, char* IPAddress);
void ClientRead(void);
void ClientSend(int x, int y);
void CloseConnection();

#endif