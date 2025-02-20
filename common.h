#pragma once

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/opensslv.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <netdb.h>

#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#endif

#define SOCKS5_VERSION 0x05            // Версия протокола SOCKS5
#define SOCKS5_CMD_CONNECT 0x01        // Команда CONNECT для SOCKS5
#define SOCKS5_ATYP_IPV4 0x01         // Тип адреса: IPv4
#define SOCKS5_ATYP_DOMAINNAME 0x03   // Тип адреса: доменное имя
#define SOCKS5_ATYP_IPV6 0x04         // Тип адреса: IPv6
#define SOCKS5_REPLY_SUCCEEDED 0x00   // Ответ об успешном соединении
#define SOCKS5_REPLY_GENERAL_FAILURE 0x01 // Ответ о общем сбое
#define SOCKS5_REPLY_NOT_ALLOWED 0x02 // Ответ, если соединение не разрешено