#ifndef __TOPOLOGY_H_INCLUDED__
#define __TOPOLOGY_H_INCLUDED__

// forward declared dependencies

// include dependencies
#include <string>
#include <vector>
#include <map>

#include <arpa/inet.h>

#include "network.h"
#include "container.h"


// declarations

#define FLAG_START "<"
#define FLAG_END ">"
#define FLAG_TERMINATE_START "</"

#define TAG_START_COMMENT "<!--"
#define TAG_END_COMMENT "-->"

#define TAG_TOPOLOGY "topology" 
#define TAG_CONTAINER "container" 
#define TAG_NETWORK "network"

#define TAG_COMMON_NAME "name"

#define TAG_CONTAINER_INTERFACE "interface"
#define TAG_CONTAINER_POSITION "position"
#define TAG_CONTAINER_OS "os"

#define TAG_INTERFACE_IP "ip"

#define TAG_NETWORK_TYPE "type"
#define TAG_NETWORK_CONNECTED "connected"

class Topology {
	std::map<std::string, ns3lxc::Network> networks;
	std::map<std::string, ns3lxc::Container> containers;
	std::map<std::string, ns3lxc::Connection> connections;
	std::map<std::string, ns3lxc::Application> applications;
};

Topology parseTopology(std::string topology_path);

#endif
