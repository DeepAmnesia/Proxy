#include "ClientHandler.h"
#include "ConnectionManager.h"

ClientHandler::ClientHandler(SOCKET clientSocket, SSL_CTX* ctx) : clientSocket(clientSocket), ssl(SSL_new(ctx)) {
    SSL_set_fd(ssl, clientSocket);
}

void ClientHandler::handle() {
    if (SSL_accept(ssl) <= 0) {
        std::cerr << "SSL handshake failed" << std::endl;
        cleanUp();
        return;
    }

    unsigned char buffer[1024];
    int bytesReceived;

    bytesReceived = SSL_read(ssl, buffer, sizeof(buffer));
    if (bytesReceived <= 0) {
        cleanUp();
        return;
    }

    if (buffer[0] != SOCKS5_VERSION) {
        SSL_write(ssl, "\x05\xFF", 2); 
        cleanUp();
        return;
    }

    SSL_write(ssl, "\x05\x00", 2);

    bytesReceived = SSL_read(ssl, buffer, sizeof(buffer));
    if (bytesReceived <= 0) {
        cleanUp();
        return;
    }

    if (buffer[1] == SOCKS5_CMD_CONNECT) {
        SOCKET targetSocket = ConnectionManager::connectToTargetServer(buffer);
        if (targetSocket == INVALID_SOCKET) {
            SSL_write(ssl, "\x05\x01\x00\x01\x00\x00\x00\x00\x00\x00", 10);
            cleanUp();
            return;
        }

        SSL_write(ssl, "\x05\x00\x00\x01\x00\x00\x00\x00\x00\x00", 10);

        char proxyBuffer[1024];
        int bytesSent;
        while (true) {
            bytesReceived = SSL_read(ssl, proxyBuffer, sizeof(proxyBuffer));
            if (bytesReceived <= 0) break;
            send(targetSocket, proxyBuffer, bytesReceived, 0);

            bytesSent = recv(targetSocket, proxyBuffer, sizeof(proxyBuffer), 0);
            if (bytesSent <= 0) break;
            SSL_write(ssl, proxyBuffer, bytesSent);
        }

        closesocket(targetSocket);
    }

    cleanUp();
}

void ClientHandler::cleanUp() {
    SSL_free(ssl);
    closesocket(clientSocket);
}