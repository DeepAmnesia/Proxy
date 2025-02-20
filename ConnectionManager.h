#pragma once
#include "common.h"

class ConnectionManager {
public:
    static SOCKET connectToTargetServer(const unsigned char* buffer);
};