#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../include/udpJsonData.h"

using namespace std;

#define SERVER_IP "192.168.10.118" // 树莓派的地址
#define PORT 30100  // 自定义端口号

int main() {
    // 创建UDP套接字
    int client_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // 设置服务器地址和端口号
    sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_address.sin_port = htons(PORT);

    while (1)
    {
        char pos[100];
        cin >> pos;
        using namespace UdpJson;
        command_pos["pos"] = pos;
        const std::string json_pos = command_pos.dump();
        sendto(client_sock, json_pos.c_str(), json_pos.size(), 0, (sockaddr*)&server_address, sizeof(server_address));

        json recv_mess_json;
        std::string recv_mess_str;

        char buffer[1024];
        socklen_t server_address_len = sizeof(server_address);
        ssize_t recv_len = recvfrom(client_sock, buffer, sizeof(buffer), 0, (sockaddr*)&server_address, &server_address_len);
        buffer[recv_len] = '\0';

        recv_mess_str = std::string(buffer);
        recv_mess_json = json::parse(recv_mess_str);

        // double recv_pos;
        // recv_pos = stol(std::string(recv_mess_json.at("position")));

        cout << "Received message: " << recv_mess_str << endl; 
    }
    
    // 关闭套接字
    close(client_sock);
    return 0;
}
