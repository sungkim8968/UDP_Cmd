#include "../include/udpJsonData.h"
#include <vector>
#include <string>

namespace UdpJson
{
    json command_pos = {{"command", "pos"},{"name", ""}, {"pos", 0.0}};
    json command_vel = {{"command", "vel"}, {"name", ""}, {"vel", 0.0}};
    json command_cur = {{"command", "cur"}, {"name", ""}, {"cur", 0.0}};
    json request_cvp = {{"request", "cvp"},{"name", ""}, {"position", 0.0}, {"velocity", 0.0}, {"current", 0.0}};
    json request_actuator_names = {{"request", "actuatour_names"}, {"actuatour_names", json::array({})}};
}