#pragma once
#include "common.h"

class SSLManager {
public:
    SSL_CTX* ctx;

    SSLManager();
    ~SSLManager();

private:
    void initializeSSL();
};