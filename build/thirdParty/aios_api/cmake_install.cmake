# Install script for directory: /home/fu/ActuatorTest/udp_robot_state/thirdParty/aios_api

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/fu/ActuatorTest/udp_robot_state/build/thirdParty/aios_api/thirdParty/SingleAxisMotion/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/fu/ActuatorTest/udp_robot_state/build/thirdParty/aios_api/src/peripheral/abs_encoder/test/cmake_install.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/home/fu/ActuatorTest/udp_robot_state/thirdParty/aios_api/lib/x86_64/libaios.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/fu/ActuatorTest/udp_robot_state/thirdParty/aios_api/lib/x86_64/libaios.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/fu/ActuatorTest/udp_robot_state/thirdParty/aios_api/lib/x86_64/libaios.so"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/fu/ActuatorTest/udp_robot_state/thirdParty/aios_api/lib/x86_64/libaios.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/home/fu/ActuatorTest/udp_robot_state/thirdParty/aios_api/lib/x86_64" TYPE SHARED_LIBRARY FILES "/home/fu/ActuatorTest/udp_robot_state/build/thirdParty/aios_api/libaios.so")
  if(EXISTS "$ENV{DESTDIR}/home/fu/ActuatorTest/udp_robot_state/thirdParty/aios_api/lib/x86_64/libaios.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/fu/ActuatorTest/udp_robot_state/thirdParty/aios_api/lib/x86_64/libaios.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/home/fu/ActuatorTest/udp_robot_state/thirdParty/aios_api/lib/x86_64/libaios.so"
         OLD_RPATH "/home/fu/ActuatorTest/udp_robot_state/thirdParty/aios_api/thirdParty/SingleAxisMotion/lib:/home/fu/ActuatorTest/udp_robot_state/thirdParty/aios_api/thirdParty/SingleAxisMotion/thirdParty/ruckig/lib/x86_64:/home/fu/ActuatorTest/udp_robot_state/build/thirdParty/aios_api/thirdParty/SingleAxisMotion:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/fu/ActuatorTest/udp_robot_state/thirdParty/aios_api/lib/x86_64/libaios.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

