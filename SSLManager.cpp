#include "SSLManager.h"

void SSLManager::initializeSSL() {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
    ctx = SSL_CTX_new(SSLv23_server_method());
    if (!ctx) {
        std::cerr << "Error creating SSL context" << std::endl;
        exit(1);
    }

    // Укажите путь к вашему SSL сертификату и ключу
    if (!SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM) ||
        !SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM)) {
        std::cerr << "Error loading certificate or key" << std::endl;
        exit(1);
    }

    if (!SSL_CTX_check_private_key(ctx)) {
        std::cerr << "Private key does not match the certificate" << std::endl;
        exit(1);
    }
}

SSLManager::SSLManager() {
    initializeSSL();
}

SSLManager::~SSLManager() {
    SSL_CTX_free(ctx);  // Очистка контекста SSL
}