cmake_minimum_required(VERSION 3.16)
project(udp_robot_state)

# include_directories(${PROJECT_SOURCE_DIR}/thirdParty/broccoli)

add_subdirectory(${PROJECT_SOURCE_DIR}/thirdParty/aios_api)

add_executable(udp_s src/udp_s.cpp src/spdlog/log.cpp src/udpJsonData.cpp)
add_executable(udp_c src/udp_c.cpp src/udpJsonData.cpp)

add_executable(udp_c_pos src/udp_send_pos.cpp src/udpJsonData.cpp)
add_executable(udp_c_vel src/udp_send_vel.cpp src/udpJsonData.cpp)
add_executable(udp_c_vel_a2 src/udp_send_vel_a2.cpp src/udpJsonData.cpp)

add_executable(udp_c_cur src/udp_send_cur.cpp src/udpJsonData.cpp)
add_executable(udp_c_cvp src/udp_req_cvp.cpp src/udpJsonData.cpp)

add_executable(udp_c_names src/udp_req_names.cpp src/udpJsonData.cpp)




target_include_directories(udp_s PUBLIC thirdParty/aios_api/include/ thirdParty/ include/)
target_include_directories(udp_c PUBLIC thirdParty/aios_api/include/ thirdParty/ include/)

target_include_directories(udp_c_pos PUBLIC thirdParty/aios_api/include/ thirdParty/ include/)
target_include_directories(udp_c_vel PUBLIC thirdParty/aios_api/include/ thirdParty/ include/)
target_include_directories(udp_c_cur PUBLIC thirdParty/aios_api/include/ thirdParty/ include/)
target_include_directories(udp_c_cvp PUBLIC thirdParty/aios_api/include/ thirdParty/ include/)
target_include_directories(udp_c_names PUBLIC thirdParty/aios_api/include/ thirdParty/ include/)
target_include_directories(udp_c_vel_a2 PUBLIC thirdParty/aios_api/include/ thirdParty/ include/)

target_link_directories(udp_s PUBLIC thirdParty/aios_aip/lib/)
target_link_directories(udp_c PUBLIC thirdParty/aios_aip/lib/)

target_link_directories(udp_c_pos PUBLIC thirdParty/aios_aip/lib/)
target_link_directories(udp_c_vel PUBLIC thirdParty/aios_aip/lib/)
target_link_directories(udp_c_cur PUBLIC thirdParty/aios_aip/lib/)
target_link_directories(udp_c_cvp PUBLIC thirdParty/aios_aip/lib/)
target_link_directories(udp_c_names PUBLIC thirdParty/aios_aip/lib/)
target_link_directories(udp_c_vel_a2 PUBLIC thirdParty/aios_aip/lib/)

target_link_libraries(udp_s aios)
target_link_libraries(udp_c aios)

target_link_libraries(udp_c_pos aios)
target_link_libraries(udp_c_vel aios)
target_link_libraries(udp_c_cur aios)
target_link_libraries(udp_c_cvp aios)
target_link_libraries(udp_c_names aios)
target_link_libraries(udp_c_vel_a2 aios)

# target_compile_features(udp_s PUBLIC cxx_std_17)
