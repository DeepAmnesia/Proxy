cmake_minimum_required(VERSION 3.10)

# Установим имя проекта
project(ProxyServer)

# Указываем стандарт C++ (например, C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем местоположение OpenSSL
find_package(OpenSSL REQUIRED)

# Добавляем исполняемый файл
add_executable(proxy_server main.cpp ClientHandler.cpp ConnectionManager.cpp SSLManager.cpp)

# Добавляем необходимые флаги для компиляции
target_include_directories(proxy_server PRIVATE ${CMAKE_SOURCE_DIR})

# Линковка с OpenSSL
target_link_libraries(proxy_server PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# Если проект зависит от дополнительных библиотек, их нужно добавить сюда
# Например, если используется pthread на Linux:
if(UNIX)
    target_link_libraries(proxy_server PRIVATE pthread)
endif()