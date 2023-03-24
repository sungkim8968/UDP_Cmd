cmake_minimum_required(VERSION 3.16)
project(udp_robot_state)

# include_directories(${PROJECT_SOURCE_DIR}/thirdParty/broccoli)

add_subdirectory(${PROJECT_SOURCE_DIR}/thirdParty/aios_api)

add_executable(udp_s src/udp_s.cpp src/spdlog/log.cpp src/udpJsonData.cpp)
add_executable(udp_c src/udp_c.cpp src/udpJsonData.cpp)


target_include_directories(udp_s PUBLIC thirdParty/aios_api/include/ thirdParty/ include/)
target_include_directories(udp_c PUBLIC thirdParty/aios_api/include/ thirdParty/ include/)


target_link_directories(udp_s PUBLIC thirdParty/aios_aip/lib/)
target_link_directories(udp_c PUBLIC thirdParty/aios_aip/lib/)

target_link_libraries(udp_s aios)
target_link_libraries(udp_c aios)

