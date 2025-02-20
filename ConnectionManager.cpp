#include "ConnectionManager.h"

SOCKET ConnectionManager::connectToTargetServer(const unsigned char* buffer) {
    sockaddr_in targetAddr4;
    sockaddr_in6 targetAddr6;
    SOCKET targetSocket = INVALID_SOCKET;

    if (buffer[3] == SOCKS5_ATYP_IPV4) {
        targetAddr4.sin_family = AF_INET;
        targetAddr4.sin_addr.s_addr = *(reinterpret_cast<unsigned long*>(&buffer[4]));
        targetAddr4.sin_port = *(reinterpret_cast<unsigned short*>(&buffer[8]));
        targetSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }
    else if (buffer[3] == SOCKS5_ATYP_DOMAINNAME) {
        uint8_t domainLength = buffer[4];
        std::string domain(reinterpret_cast<char*>(&buffer[5]), domainLength);
        uint16_t port = ntohs(*(reinterpret_cast<uint16_t*>(&buffer[5 + domainLength])));

        struct hostent* host = gethostbyname(domain.c_str());
        if (!host) {
            std::cerr << "Error resolving domain: " << domain << std::endl;
            return INVALID_SOCKET;
        }

        targetAddr4.sin_family = AF_INET;
        targetAddr4.sin_addr.s_addr = *(reinterpret_cast<unsigned long*>(host->h_addr));
        targetAddr4.sin_port = port;
        targetSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }
    else if (buffer[3] == SOCKS5_ATYP_IPV6) {
        targetAddr6.sin6_family = AF_INET6;
        memcpy(&targetAddr6.sin6_addr, &buffer[4], 16); 
        targetAddr6.sin6_port = *(reinterpret_cast<unsigned short*>(&buffer[20]));
        targetSocket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    }

    if (targetSocket != INVALID_SOCKET) {
        int res;
        if (buffer[3] == SOCKS5_ATYP_IPV6) {
            res = connect(targetSocket, (sockaddr*)&targetAddr6, sizeof(targetAddr6));
        }
        else {
            res = connect(targetSocket, (sockaddr*)&targetAddr4, sizeof(targetAddr4));
        }

        if (res == SOCKET_ERROR) {
            std::cerr << "Error connecting to target server" << std::endl;
            return INVALID_SOCKET;
        }
    }

    return targetSocket;
}