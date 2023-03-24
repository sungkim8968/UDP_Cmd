#pragma once

#include "nlohmann/json.hpp"

using namespace nlohmann;

namespace FourierDynamics
{
  namespace AIOS
  {
    namespace JSONData
    {
      extern json get_aios_root_attribute_json;
      extern json enable_json;
      extern json disable_json;
      extern json set_control_mode_json;
      extern json get_cvp_json;
      extern json get_trapezoidal_trajectory_param_json;
      extern json set_trapezoidal_trajectory_param_json;
      extern json get_motion_controller_config_json;
      extern json set_motion_controller_config_json;
      extern json move_to_json;
      extern json set_linear_count_json;
      extern json ext_pos_json;
      extern json ext_vel_json;
      extern json ext_cur_json;
      extern json vel_ramp_enable_json;
      extern json set_vel_ramp_target_json;
    }
  }
}