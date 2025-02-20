#pragma once
#include "common.h"

class ClientHandler {
public:
    ClientHandler(SOCKET clientSocket, SSL_CTX* ctx);
    void handle();

private:
    SOCKET clientSocket;
    SSL* ssl;
    void cleanUp();
};