#include <iostream>
#include <thread>

#include <list>
#include <map>

#include <cstring>
#include <unistd.h>
#include <vector>

#include "aios/aios.h"
#include "spdlog/log.h"

#include "../include/udpJsonData.h"
#include "../include/udp_s.hpp"

#define VAR_NAME(x) #x

// 两个线程，一个专门发送命令，一个接受信息
using namespace udpServer::Server;
using namespace UdpJson;

std::map< std::string, std::string> map_ips;
void add_ip()
{
    map_ips["10.10.10.16"] = "a1";
    map_ips["10.10.10.190"] = "a2";
}

static const double cycle_time_s = 0.0025;
static const double gear_ratio = 51.0;
static const double g = 9.81;
static const double max_pos = 100000.0;   
static const double min_pos = -100000.0;
static const double max_vel = 10.0;
static const double max_acc = 40.0;

static const double max_jerk = 160.0;
static const double load_mass = 8.25;
static const double link_length = 0.35;
std::string a1_ip = "10.10.10.16";
FourierDynamics::AIOS::AIOS a1(a1_ip, cycle_time_s, max_pos, min_pos, max_vel, 4 * max_vel, 16 * max_vel);

std::string a2_ip = "10.10.10.190";
FourierDynamics::AIOS::AIOS a2(a2_ip, cycle_time_s, max_pos, min_pos, max_vel, 4 * max_vel, 16 * max_vel);

void cycle_loop(void (*fun_pt)(), double cycle_time_s){
    broccoli::core::Time timer;
    broccoli::core::Time start_time;
    broccoli::core::Time end_time;
    broccoli::core::Time last_time;
    broccoli::core::Time time_to_sleep;
    broccoli::core::Time cycle_time{0, cycle_time_s*1e9};
    timespec time_to_sleep_std;
    while (1)
    {
        start_time = timer.currentTime();
        fun_pt();
        time_to_sleep = start_time + cycle_time;
        time_to_sleep_std = time_to_sleep.toTimeSpec();
        last_time = start_time;
        
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &time_to_sleep_std, NULL);
    }
    
};



void udpserver(){
    // sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT);
    bind(server_sock, (sockaddr*)&server_address, sizeof(server_address));

    memset(&client_address, 0, sizeof(client_address));
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = htonl(INADDR_ANY);
    client_address.sin_port = htons(0);
}  

// 发送命令线程

void serverSend(FourierDynamics::AIOS::AIOS name, int sockfd, struct sockaddr_in clientaddr)
{
    request_cvp["position"] = name.get_act_pos_deg()*51.0;
    request_cvp["velocity"] = name.get_act_vel_deg();
    request_cvp["current"] = name.get_act_cur();
    const std::string json_pos = request_cvp.dump();
    cout << "send  message: " << json_pos << endl; 
    int nbytes = sendto(sockfd, json_pos.c_str(), json_pos.size(), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));

    if (nbytes < 0)
    {
        std::cerr << "send error" << std::endl;
        // break;
    }
    std::cout << "Sent " << nbytes << " bytes to " << inet_ntoa(clientaddr.sin_addr) << ":" << ntohs(clientaddr.sin_port) << std::endl;
}

void choose_cmd(FourierDynamics::AIOS::AIOS act, json j)
{
    double ext_pos;
    double ext_vel;
    double ext_cur;
    if (j.contains("command"))
        {
            if (j.at("command") == "pos")
            {
                act.EnableExtPos();
                std::cout << "receive position:  "<< j.at("pos") << std::endl;
                ext_pos = j.at("pos");
                act.SetExtPos(ext_pos/51.0, 0.0, 0.0);
            }
            if (j.at("command") == "vel")
            {
                act.EnableExtVel();
                std::cout << "receive velocity:  "<< j.at("vel") << std::endl;
                ext_vel = j.at("vel");
                act.SetExtVel(ext_vel, 0.0);
            }
            if (j.at("command") == "cur")
            {
                act.EnableExtCur();
                std::cout << "receive current:  "<< j.at("cur") << std::endl;
                ext_cur = j.at("cur");
                act.SetExtCur(ext_cur);
        }
    }
    if (j.contains("request"))
    {
        if (j.at("request") == "cvp")
        {
            request_cvp["position"] = act.get_act_pos_deg()*51.0;
            request_cvp["velocity"] = act.get_act_vel_deg();
            request_cvp["current"] = act.get_act_cur();
            const std::string json_pos = request_cvp.dump();
            cout << "send  message: " << json_pos << endl;

            int nbytes = sendto(server_sock, json_pos.c_str(), json_pos.size(), 0, (struct sockaddr*)&client_address, sizeof(client_address));
            if (nbytes < 0)
            {
                std::cerr << "send error" << std::endl;
            }
            std::cout << "Sent " << nbytes << " bytes to " << inet_ntoa(client_address.sin_addr) << ":" << ntohs(client_address.sin_port) << std::endl; 
        }
    }
}


// 电机
void task(){
    enum STATE{init, working};
    enum INIT_STATE{get_attr, set_gear_ration, enable, init_done};
    enum WORK_STATE{prepare_run, run, done, error};


    static STATE state{STATE::init};
    static INIT_STATE init_state{INIT_STATE::get_attr};
    static WORK_STATE work_state{WORK_STATE::prepare_run};


    static double ki = 0.054;
    double load_gravity_torque = 0.0;
    double motor_current_ff = 0.0;

    int ret_a1 = 0;
    int ret_a2 = 0;

    udpserver();

    // 通信实例化
    
    json recv_mess_json;
    std::string recv_mess_str;
    
    switch (state)
    {
    case STATE::init:

        switch (init_state)
        {

        case INIT_STATE::get_attr:

            ret_a1 = 0;
            ret_a1 = a1.init->Init();
            ret_a2 = 0;
            ret_a2 = a2.init->Init();
            if (ret_a1>0 && ret_a2>0)
            {
                init_state = INIT_STATE::set_gear_ration;
                std::cout<< "init_state -> " << init_state <<std::endl; 
            }
            break;
            std::cout<<"开始设置齿轮比" << std::endl;
        case INIT_STATE::set_gear_ration:
            a1.SetGearRatio(51.0);
            a2.SetGearRatio(51.0);
            init_state = INIT_STATE::enable;
            break;
        case INIT_STATE::enable:
            ret_a1 = 0;
            ret_a2 = 0;
            ret_a1 = a1.power->PowerOn();
            ret_a2 = a2.power->PowerOn();
            if (ret_a1>0 && ret_a2>0)
            {
                init_state = INIT_STATE::init_done;
                state = STATE::working;
                
                std::cout << "init enable state -> " << init_state <<std::endl;
            }
            break;
        case INIT_STATE::init_done:
            break;
        }
        break;
    
    case STATE::working:
        a1.UpdateInputRTData();
        a2.UpdateInputRTData();
        
        switch (work_state)
        {
        case WORK_STATE::prepare_run:
            work_state = WORK_STATE::run;
            break;
        case WORK_STATE::run:
        // 接收到的信息
            ssize_t recv_len = recvfrom(server_sock, client_buffer, sizeof(client_buffer), 0, (sockaddr*)&client_address, &client_address_len);
            client_buffer[recv_len] = '\0';

            recv_mess_str = std::string(client_buffer);
            cout << " Received message: " << recv_mess_str << endl;
            recv_mess_json = json::parse(recv_mess_str);

            if (recv_mess_json.contains("actuatour_names"))
            {
                std::vector<std::string> name_list;
                // 对 aios_lists 进行处理，并获取关键名称，将其转化成 json array 格式。
                for (const auto c : map_ips) 
                {
                    std::cout << c.second << std::endl;
                    name_list.push_back(c.second);
                }
                request_actuator_names["actuatour_names"] = name_list;
                std::cout << request_actuator_names.dump(4) << std::endl;
                const std::string json_j = request_actuator_names.dump();
                int nbytes = sendto(server_sock, json_j.c_str(), json_j.size(), 0, (struct sockaddr*)&client_address, sizeof(client_address));

                if (nbytes < 0)
                {
                    std::cerr << "send error" << std::endl;
                    // break;
                }
                std::cout << "Sent " << nbytes << " bytes to " << inet_ntoa(client_address.sin_addr) << ":" << ntohs(client_address.sin_port) << std::endl;
            }
            if (recv_mess_json["name"] == "a1")
            {
                double ext_pos;
                double ext_vel;
                double ext_cur;
                if (recv_mess_json.contains("command"))
                    {
                        if (recv_mess_json.at("command") == "pos")
                        {
                            a1.EnableExtPos();
                            std::cout << "receive position:  "<< recv_mess_json.at("pos") << std::endl;
                            ext_pos = recv_mess_json.at("pos");
                            a1.SetExtPos(ext_pos/51.0, 0.0, 0.0);
                        }
                        if (recv_mess_json.at("command") == "vel")
                        {
                            a1.EnableExtVel();
                            std::cout << "receive velocity:  "<< recv_mess_json.at("vel") << std::endl;
                            ext_vel = recv_mess_json.at("vel");
                            a1.SetExtVel(ext_vel, 0.0);
                        }
                        if (recv_mess_json.at("command") == "cur")
                        {
                            a1.EnableExtCur();
                            std::cout << "receive current:  "<< recv_mess_json.at("cur") << std::endl;
                            ext_cur = recv_mess_json.at("cur");
                            a1.SetExtCur(ext_cur);
                    }
                }
                if (recv_mess_json.contains("request"))
                {
                    if (recv_mess_json.at("request") == "cvp")
                    {
                        request_cvp["position"] = a1.get_act_pos_deg()*51.0;
                        request_cvp["velocity"] = a1.get_act_vel_deg();
                        request_cvp["current"] = a1.get_act_cur();
                        const std::string json_pos = request_cvp.dump();
                        cout << "send  message: " << json_pos << endl;

                        int nbytes = sendto(server_sock, json_pos.c_str(), json_pos.size(), 0, (struct sockaddr*)&client_address, sizeof(client_address));
                        if (nbytes < 0)
                        {
                            std::cerr << "send error" << std::endl;
                        }
                        std::cout << "Sent " << nbytes << " bytes to " << inet_ntoa(client_address.sin_addr) << ":" << ntohs(client_address.sin_port) << std::endl; 
                    }
                }

            }
            if (recv_mess_json["name"] == "a2")
            {
                double ext_pos;
                double ext_vel;
                double ext_cur;
                if (recv_mess_json.contains("command"))
                    {
                        if (recv_mess_json.at("command") == "pos")
                        {
                            a2.EnableExtPos();
                            std::cout << "receive position:  "<< recv_mess_json.at("pos") << std::endl;
                            ext_pos = recv_mess_json.at("pos");
                            a2.SetExtPos(ext_pos/51.0, 0.0, 0.0);
                        }
                        if (recv_mess_json.at("command") == "vel")
                        {
                            a2.EnableExtVel();
                            std::cout << "receive velocity:  "<< recv_mess_json.at("vel") << std::endl;
                            ext_vel = recv_mess_json.at("vel");
                            a2.SetExtVel(ext_vel, 0.0);
                        }
                        if (recv_mess_json.at("command") == "cur")
                        {
                            a2.EnableExtCur();
                            std::cout << "receive current:  "<< recv_mess_json.at("cur") << std::endl;
                            ext_cur = recv_mess_json.at("cur");
                            a2.SetExtCur(ext_cur);
                    }
                }
                if (recv_mess_json.contains("request"))
                {
                    if (recv_mess_json.at("request") == "cvp")
                    {
                        request_cvp["position"] = a2.get_act_pos_deg()*51.0;
                        request_cvp["velocity"] = a2.get_act_vel_deg();
                        request_cvp["current"] = a2.get_act_cur();
                        const std::string json_pos = request_cvp.dump();
                        cout << "send  message: " << json_pos << endl;

                        int nbytes = sendto(server_sock, json_pos.c_str(), json_pos.size(), 0, (struct sockaddr*)&client_address, sizeof(client_address));
                        if (nbytes < 0)
                        {
                            std::cerr << "send error" << std::endl;
                        }
                        std::cout << "Sent " << nbytes << " bytes to " << inet_ntoa(client_address.sin_addr) << ":" << ntohs(client_address.sin_port) << std::endl; 
                    }
                }

            }

            break;
        }
        
        a1.UpdateOutputRTData();
        a2.UpdateOutputRTData();
        break;
    }
}


void serverRecv(int sockfd, struct sockaddr_in clientaddr)
{
    while (true)
    {

    int addrlen = sizeof(clientaddr);

    int nbytes = recvfrom(sockfd, recv_buffer, BUFFER_SIZE, 0, (struct sockaddr*)&clientaddr, (socklen_t*)&addrlen);
    if (nbytes <0)
    {
        std::cerr << "recv error " << std::endl;
    }
    recv_buffer[nbytes] = '\0';
    std::cout << "Recv from " << inet_ntoa(clientaddr.sin_addr) << ":" << ntohs(clientaddr.sin_port) << ": " << recv_buffer << endl;
    
    json recv_mess_json;
    std::string recv_mess_str;
    recv_mess_str = std::string(recv_buffer);

    cout << " Received message: " << recv_mess_str << endl;
    recv_mess_json = json::parse(recv_mess_str);

    if (recv_mess_json.contains("actuatour_names"))
    {
        std::vector<std::string> name_list;
        // 对 aios_lists 进行处理，并获取关键名称，将其转化成 json array 格式。
        for (const auto c : map_ips) 
        {
            std::cout << c.second << std::endl;
            name_list.push_back(c.second);
        }
        request_actuator_names["actuatour_names"] = name_list;
        std::cout << request_actuator_names.dump(4) << std::endl;
        const std::string json_j = request_actuator_names.dump();
        int nbytes = sendto(sockfd, json_j.c_str(), json_j.size(), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));

        if (nbytes < 0)
        {
            std::cerr << "send error" << std::endl;
            // break;
        }
        std::cout << "Sent " << nbytes << " bytes to " << inet_ntoa(clientaddr.sin_addr) << ":" << ntohs(clientaddr.sin_port) << std::endl;
    }
    
    
    }
}

enum SWITCH_THREAD{on, off};
static SWITCH_THREAD switch_thread{SWITCH_THREAD::off};




int main() {
    FourierDynamics::Log::SPDLOG mylog;
#ifdef _MY_DEBUG
    mylog.setLevel(spdlog::level::level_enum::debug);
#else
    mylog.setLevel(spdlog::level::level_enum::info);
#endif

    // 多电机实例化
    add_ip();
    std::thread cycle_thread(cycle_loop, task, 0.0025);
    cycle_thread.join();
    
}





