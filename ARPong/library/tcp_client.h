#pragma once
#include "tcp_message.h"

bool ConnectToHost(int PortNo, char* IPAddress);
void ClientRead(void);
void ClientWrite(float x, float y);
void CloseConnection();