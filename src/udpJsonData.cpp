#include "../include/udpJsonData.h"
#include <vector>
#include <string>

namespace UdpJson
{
    json command_pos = {{"command", "pos"}, {"pos", 0}};
    json command_vel = {{"command", "vel"}, {"vel", 0}};
    json command_cur = {{"command", "cur"}, {"cur", 0}};
    json request_cvp = {{"request", "cvp"},{"position", 0.0}, {"velocity", 0.0}, {"current", 0.0}};
    json request_actuator_names = {{"request", "actuatour_names"}, {"actuatour_names", json::array({})}};
}