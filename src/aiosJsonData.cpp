#include "aios/aiosJsonData.h"
namespace FourierDynamics {
namespace AIOS {
namespace JSONData {
json get_aios_root_attribute_json = {{"method", "GET"}, {"reqTarget", "/"}};

json enable_json = {{"method", "SET"}, {"reqTarget", "/m1/requested_state"}, {"property", 8}};

json disable_json = {{"method", "SET"}, {"reqTarget", "/m1/requested_state"}, {"property", 1}};

json set_control_mode_json = {{"method", "SET"}, {"reqTarget", "/m1/controller/config"}, {"control_mode", 1}};

json get_cvp_json = {{"method", "GET"}, {"reqTarget", "/m1/CVP"}};

json get_trapezoidal_trajectory_param_json = {{"method", "GET"}, {"reqTarget", "/m1/trap_traj"}};

json set_trapezoidal_trajectory_param_json = {{"method", "SET"},
                                              {"reqTarget", "/m1/trap_traj"},
                                              {"accel_limit", 20000},
                                              {"decel_limit", 20000},
                                              {"vel_limit", 60000}};

json get_motion_controller_config_json = {{"method", "GET"}, {"reqTarget", "/m1/controller/config"}};

json set_motion_controller_config_json = {
    {"method", "SET"},           {"reqTarget", "/m1/controller/config"}, {"pos_gain", 20},
    {"vel_gain", 0.0005},        {"vel_integrator_gain", 0.0002},        {"vel_limit", 400000},
    {"vel_limit_tolerance", 1.2}};

json move_to_json = {{"method", "SET"}, {"reqTarget", "/m1/trapezoidalMove"}, {"property", 0}, {"reply_enable", false}};

json set_linear_count_json = {{"method", "SET"}, {"reqTarget", "/m1/encoder"}, {"set_linear_count", 0}};

json ext_pos_json = {
    {"method", "SET"}, {"reqTarget", "/m1/setPosition"}, {"position", 0}, {"velocity_ff", 0}, {"current_ff", 0}};

json ext_vel_json = {{"method", "SET"}, {"reqTarget", "/m1/setVelocity"}, {"velocity", 0}, {"current_ff", 0}};

json ext_cur_json = {
    {"method", "SET"},
    {"reqTarget", "/m1/setCurrent"},
    {"current", 0},
};

json vel_ramp_enable_json = {
    {"method", "SET"},
    {"reqTarget", "/m1/controller"},
    {"vel_ramp_enable", true},
};

json set_vel_ramp_target_json = {
    {"method", "SET"},
    {"reqTarget", "/m1/controller"},
    {"vel_ramp_target", 0},
};
}  // namespace JSONData
}  // namespace AIOS
}  // namespace FourierDynamics