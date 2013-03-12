#pragma once
#include "tcp_message.h"

bool ConnectToHost(int PortNo, char* IPAddress);
server_data ClientRead(void);
void ClientWrite(float x);
void CloseConnection();