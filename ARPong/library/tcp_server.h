#pragma once
#include "tcp_message.h"

#define SOCK_STREAM      1
#define SOCK_DGRAM      2
#define SOCK_RAW      3
#define AF_INET      2
#define IPPROTO_TCP      6

int ListenOnPort(int portno);
client_data ServerRead(void);
void ServerWrite(float cx, float cy, float bx, float by, float bz);