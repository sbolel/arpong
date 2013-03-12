#pragma once
#include "tcp_message.h"

int ListenOnPort(int portno);
client_data ServerRead(void);
void ServerWrite(glm::vec2 paddle, glm::vec3 ball);