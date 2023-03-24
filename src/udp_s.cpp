#include <iostream>
#include <thread>

#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "aios/aios.h"
#include "spdlog/log.h"
#include "../include/udpJsonData.h"


#define PORT 30100

// 全局
int server_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
sockaddr_in server_address;

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

void udpServer(){
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT);
    bind(server_sock, (sockaddr*)&server_address, sizeof(server_address));
}

void task(){
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
    static const std::string actuator_ip = "10.10.10.16";
    static FourierDynamics::AIOS::AIOS actuator1(actuator_ip,cycle_time_s,max_pos, min_pos, max_vel, 4*max_vel,16*max_vel);

    enum STATE{init, working};
    enum INIT_STATE{get_attr, set_gear_ration, enable, init_done};
    enum WORK_STATE{prepare_run, run, done, error};
    static STATE state{STATE::init};
    static INIT_STATE init_state{INIT_STATE::get_attr};
    static WORK_STATE work_state{WORK_STATE::prepare_run};

    static double ki = 0.054;
    double load_gravity_torque = 0.0;
    double motor_current_ff = 0.0;

    int ret = 0;

    // 建立udp连接
    udpServer();

    // 创建客户端
    char buffer[1024];
    sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    // 初始化并使能
    switch (state)
    {
    case STATE::init:
        switch (init_state)
        {

        case INIT_STATE::get_attr:
            ret = 0;
            ret = actuator1.init->Init();
            if (ret>0)
            {
                init_state = INIT_STATE::set_gear_ration;
                std::cout<< "init_state -> " << init_state <<std::endl; 
            }
            break;
            std::cout<<"开始设置齿轮比" << std::endl;
        case INIT_STATE::set_gear_ration:
            actuator1.SetGearRatio(gear_ratio);
            init_state = INIT_STATE::enable;
            break;
        case INIT_STATE::enable:
            ret = 0;
            ret = actuator1.power->PowerOn();
            if (ret>0)
            {
                init_state = INIT_STATE::init_done;
                state = STATE::working;
                std::cout << "init state -> " << init_state <<std::endl;
            }
            break;
        case INIT_STATE::init_done:
            break;
        }
        break;
    
    case STATE::working:
        using namespace UdpJson;
        actuator1.UpdateInputRTData();
        switch (work_state)
        {
        case WORK_STATE::prepare_run:
            actuator1.EnableExtPos();
            actuator1.SetExtPos(0.0, 0.0, 0.0);
            work_state = WORK_STATE::run;
            break;
        case WORK_STATE::run:

        // 接收到的信息
            double ext_pos;
            
            json recv_mess_json;
            std::string recv_mess_str;
            
            ssize_t recv_len = recvfrom(server_sock, buffer, sizeof(buffer), 0, (sockaddr*)&client_address, &client_address_len);
            buffer[recv_len] = '\0';

            recv_mess_str = std::string(buffer);
            recv_mess_json = json::parse(recv_mess_str);

            ext_pos = stol(std::string(recv_mess_json.at("pos")));

            cout << " Received message: " << recv_mess_str << endl;
            actuator1.SetExtPos(ext_pos/gear_ratio, 0.0, 0.0);
            std::cout<< "current_pos: " << actuator1.get_act_pos_deg()*gear_ratio << "  current_ff: " <<
                motor_current_ff << "  current_fb: " << actuator1.get_act_cur() << std::endl;

            // 消息处理并执行相应动作
            // char pos[10];
            // sprintf(pos, "%f", actuator1.get_act_pos_deg()*gear_ratio);

            request_cvp["position"] = actuator1.get_act_pos_deg()*gear_ratio;
            const std::string json_pos = request_cvp.dump();
            cout << "sned  message: " << json_pos << endl; 
            sendto(server_sock, json_pos.c_str(), json_pos.size(), 0, (sockaddr*)&client_address, client_address_len);
            break;
        }
        actuator1.UpdateOutputRTData();
        break;
    }
}


int main() {
    FourierDynamics::Log::SPDLOG mylog;
#ifdef _MY_DEBUG
    mylog.setLevel(spdlog::level::level_enum::debug);
#else
    mylog.setLevel(spdlog::level::level_enum::info);
#endif
    std::thread cycle_thread(cycle_loop, task, 0.0025);
    cycle_thread.join();
    cycle_loop(task, 0.0025);
}





