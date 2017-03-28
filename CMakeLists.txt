# Author: Michael Albert

cmake_minimum_required(VERSION 3.3)
project(NS-3_LXC)

message(Building Executables.)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

include_directories(
        /usr/src/linux-headers-4.4.0-63/
        src/main/include
        src/main/parse/include
        src/lib/include
)

set(Local_Libs
		src/lib/include/ipaddr.h
		src/lib/include/node.h
		src/lib/include/iface.h
		src/lib/include/position.h
		src/lib/include/link.h
		src/lib/include/topology.h
		src/lib/include/application.h
		src/lib/ipaddr.cpp
        src/lib/node.cpp
		src/lib/link.cpp
        src/lib/topology.cpp
        src/lib/iface.cpp
)

set(NS-3_LXC
        src/main/parse/include/settingsParser.h
    	src/main/parse/include/topologyParser.h
        src/main/parse/include/nodeParser.h
        src/main/parse/include/linkParser.h
    	src/main/parse/topologyParser.cpp
        src/main/parse/settingsParser.cpp
        src/main/parse/nodeParser.cpp
        src/main/parse/linkParser.cpp
        src/main/include/logger.h
        src/main/logger.cpp
        src/main/main.cpp
)

add_executable(ns3lxc ${NS-3_LXC} ${Local_Libs})

find_library(YAML_LIB yaml-cpp)

target_link_libraries(ns3lxc ${YAML_LIB})


target_compile_options(ns3lxc PUBLIC -std=c++11 -pthread -g -Wfatal-errors)