# Author: Michael Albert

cmake_minimum_required(VERSION 3.3)
project(NS-3_LXC)

message(Building Executables.)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

include_directories(
        /usr/src/linux-headers-4.4.0-63/
        src/main/include
        src/lib/include
)

set(Local_Libs
		src/lib/include/ipaddr.h
		src/lib/include/container.h
		src/lib/include/iface.h
		src/lib/include/position.h
		src/lib/include/connection.h
		src/lib/include/network.h
		src/lib/include/application.h
		src/lib/ipaddr.cpp
		src/lib/connection.cpp
        src/lib/network.cpp
        src/lib/iface.cpp
)

set(NS-3_LXC
        src/main/include/settings.h
    	src/main/include/topology.h
    	src/main/topology.cpp
        src/main/settings.cpp
        src/main/main.cpp)

add_executable(ns3lxc ${NS-3_LXC} ${Local_Libs})

find_library(YAML_LIB yaml-cpp)

target_link_libraries(ns3lxc ${YAML_LIB})


target_compile_options(ns3lxc PUBLIC -std=c++11 -pthread)