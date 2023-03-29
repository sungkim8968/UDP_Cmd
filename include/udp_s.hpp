#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>
#include <list>
#include <map>
#include "aios/aios.h"
#include "spdlog/log.h"

#define PORT 30100
#define BUFFER_SIZE 1024



// std::map<std::string, FourierDynamics::AIOS::AIOS> actuator_map;
namespace udpServer
{
    namespace Server
    {
        int server_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        sockaddr_in server_address;
        sockaddr_in client_address;
        char client_buffer[BUFFER_SIZE];
        char send_buffer[BUFFER_SIZE];
        char recv_buffer[BUFFER_SIZE];
        socklen_t client_address_len = sizeof(client_address);

        std::list<std::map< std::string, std::string>> actuator_lists;

        std::vector<FourierDynamics::AIOS::AIOS> aios_lists;
        // FourierDynamics::AIOS::AIOS a1;
        // FourierDynamics::AIOS::AIOS a2;

    }
}