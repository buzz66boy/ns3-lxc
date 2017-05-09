# Author: Michael Albert

cmake_minimum_required(VERSION 3.3)
project(NS-3_LXC)

message(Building Executables.)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

include_directories(
        /usr/src/linux-headers-generic/
        /usr/include
        src/main/include
        src/main/parse/include
        src/main/generation/include
        src/lib/include
        src/links/include
        src/applications/include
        src/containers/includes
)

set(Link_Types
        src/links/include/linkType.h
        src/links/include/linkTypeMap.h
        src/links/linkTypeMap.cpp
        src/links/include/csma.h
        src/links/include/wifi.h
        src/links/csma.cpp
        src/links/wifi.cpp
        # Add your link types here (both .h and .cpp)
)

set(Application_Types
        src/applications/include/applicationType.h
        src/applications/include/applicationTypeMap.h
        # Add your custom app files here
)

set(Container_Types
        src/containers/includes/containerType.h
        src/containers/includes/lxcContainerType.h
        src/containers/lxcContainerType.cpp
        # Add custom container handlers here
)

set(Local_Libs
		src/lib/include/ipaddr.h
		src/lib/include/node.h
		src/lib/include/iface.h
		src/lib/include/position.h
		src/lib/include/link.h
		src/lib/include/topology.h
		src/lib/include/application.h
        src/lib/include/nameable.h
		src/lib/ipaddr.cpp
        src/lib/node.cpp
		src/lib/link.cpp
        src/lib/topology.cpp
        src/lib/iface.cpp
)

set(NS-3_LXC
        src/main/parse/include/parserTags.h
        src/main/parse/include/settingsParser.h
    	src/main/parse/include/topologyParser.h
        src/main/parse/include/subTopologyParser.h
        src/main/parse/include/nodeParser.h
        src/main/parse/include/linkParser.h
        src/main/parse/parserTags.cpp
    	src/main/parse/topologyParser.cpp
        src/main/parse/subTopologyParser.cpp
        src/main/parse/settingsParser.cpp
        src/main/parse/nodeParser.cpp
        src/main/parse/linkParser.cpp
        src/main/generation/include/bridgeTapBuilder.h
        src/main/generation/include/nodeSpawner.h
        src/main/generation/include/ns3Writer.h
        src/main/generation/include/topologyGenerator.h
        src/main/generation/bridgeTapBuilder.cpp
        src/main/generation/nodeSpawner.cpp
        src/main/generation/ns3Writer.cpp
        src/main/generation/topologyGenerator.cpp
        src/main/include/logger.h
        src/main/logger.cpp
        src/main/main.cpp
)

add_executable(ns3lxc ${NS-3_LXC} ${NS-3_Template_Write} ${Local_Libs} ${Link_Types} ${Container_Types} ${Application_Types})

find_library(YAML_LIB yaml-cpp)

find_library(LXC_LIB lxc)

target_link_libraries(ns3lxc ${YAML_LIB} ${LXC_LIB})


target_compile_options(ns3lxc PUBLIC -std=c++11 -pthread -g -Wfatal-errors -pipe)