#pragma once

#include "nlohmann/json.hpp"

using namespace nlohmann;

namespace UdpJson
{
    extern json command_pos;
    extern json command_vel;
    extern json command_cur;
    extern json request_cvp;
    extern json request_actuator_names;
}