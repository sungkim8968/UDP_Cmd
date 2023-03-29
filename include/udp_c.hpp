#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 30100 // 自定义端口号
#define BUFFER_SIZE 1024

namespace udpClient
{
    namespace Client
    {
        int client_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        sockaddr_in server_address;
        char recv_buffer[BUFFER_SIZE];
        char send_buffer[BUFFER_SIZE];
    }
}