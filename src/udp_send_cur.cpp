#include <iostream>
#include <cstring>
#include <unistd.h>
#include <thread>

#include "../include/udpJsonData.h"
#include "../include/udp_c.hpp"


#define SERVER_IP "192.168.10.216" // 树莓派的地址  在测试时，暂用本机地址

using namespace std;
using namespace udpClient::Client;
using namespace UdpJson;

// 两个线程，一个专门

enum SWITCH_THREAD{on, off};
static SWITCH_THREAD switch_thread{SWITCH_THREAD::off};

// UDP 初始化
void udpInit()
{
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_address.sin_port = htons(PORT);
}


// 接受命令线程  解析数据
void clientRecv(int sockfd, struct sockaddr_in serveraddr)
{
    json recv_mess_json;
    std::string recv_mess_str;
    socklen_t server_address_len = sizeof(serveraddr);

    while (true)
    {
        int recv_len = recvfrom(client_sock, recv_buffer, sizeof(recv_buffer), 0, (sockaddr*)&server_address, &server_address_len);
        if (recv_len <0)
        {
            std::cerr << "recv error " << std::endl;
            break;
        }
        recv_buffer[recv_len] = '\0';

        recv_mess_str = std::string(recv_buffer);
        recv_mess_json = json::parse(recv_mess_str);

        double recv_pos;
        recv_pos = recv_mess_json.at("position");
        double recv_vel;
        recv_vel = recv_mess_json.at("velocity");
        double recv_cur;
        recv_cur = recv_mess_json.at("current");

        // cout << "Position: " <<  recv_pos << " Velocity: " << recv_vel << "Current: " << recv_cur << endl;
        cout << "Recv from " << inet_ntoa(serveraddr.sin_addr) << ":" << ntohs(serveraddr.sin_port) << ": " << recv_buffer << endl;
    }
}

// 选取下发模式  位置 速度 电流

void commandPos(double pos)
{

    command_pos["pos"] = pos;
    const string json_pos = command_pos.dump();
    sendto(client_sock, json_pos.c_str(), json_pos.size(), 0, (sockaddr*)&server_address, sizeof(server_address));
    switch_thread = SWITCH_THREAD::on;
    
}

void commandVel(double vel)
{
    command_vel["vel"] = vel;
    const string json_vel = command_vel.dump();
    sendto(client_sock, json_vel.c_str(), json_vel.size(), 0, (sockaddr*)&server_address, sizeof(server_address));
    switch_thread = SWITCH_THREAD::on; 

}

void commandCur()
{
    while (true)
    {
        cin >> send_buffer;
        double tmp_cur = atof(send_buffer);
        command_cur["cur"] = tmp_cur;
        const string json_cur = command_cur.dump();
        sendto(client_sock, json_cur.c_str(), json_cur.size(), 0, (sockaddr*)&server_address, sizeof(server_address));
        switch_thread = SWITCH_THREAD::on;
        std::cout << "send message: " << json_cur << std::endl;
    }
}


void startThread()
{
    std::thread recv_mess(clientRecv, client_sock, server_address);
    // switch_thread = SWITCH_THREAD::off;
    recv_mess.join();
}

void requestCVP()
{
    request_cvp["request"] = "cvp";
    const string json_cvp = request_cvp.dump();
    sendto(client_sock, json_cvp.c_str(), json_cvp.size(), 0, (sockaddr*)&server_address, sizeof(server_address));
    switch_thread = SWITCH_THREAD::on; 
}

int main() {
    udpInit();

    thread send(commandCur);
    thread recv(startThread);
    
    send.join();
    if (switch_thread == SWITCH_THREAD::on)
    {
        recv.join();
    }

    close(client_sock);
    return 0;
}
