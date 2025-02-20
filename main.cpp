#include "common.h"
#include "SSLManager.h"
#include "ClientHandler.h"

std::mutex thread_mutex; 

#ifdef WIN32
void initializeWinSock() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed!" << std::endl;
        exit(1);
    }
}
#endif

class ProxyServer {
public:
    ProxyServer(int port) : port(port) {}

    void start() {
        SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (serverSocket == INVALID_SOCKET) {
            std::cerr << "Error creating socket" << std::endl;
            return;
        }

        sockaddr_in serverAddr = {};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(port);

        if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "Bind failed" << std::endl;
            return;
        }

        if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
            std::cerr << "Listen failed" << std::endl;
            return;
        }

        std::cout << "Listening on port " << port << "..." << std::endl;

        while (true) {
            SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
            if (clientSocket == INVALID_SOCKET) {
                std::cerr << "Accept failed" << std::endl;
                continue;
            }

            std::lock_guard<std::mutex> lock(thread_mutex);
            std::thread(&ProxyServer::handleClient, this, clientSocket).detach();
        }

        closesocket(serverSocket);
    }

private:
    int port;

    void handleClient(SOCKET clientSocket) {
        ClientHandler(clientSocket, sslManager.ctx).handle();
    }

    SSLManager sslManager;
};

int main() {
#ifdef WIN32
    initializeWinSock();
#endif

    ProxyServer server(443);
    server.start();
    return 0;
}