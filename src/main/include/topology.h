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
#include "application.h"

// declarations

// NOTE: these tags can also be used plurally (checks for tag + "s")
#define TAG_INCLUDE "include" 
#define TAG_CONTAINER "container" 
#define TAG_NETWORK "network"
#define TAG_APPLICATION "application"
#define TAG_CONNECTION "connection"
#define TAG_POSITION "position"
#define TAG_IFACE "iface"
#define TAG_IFACES_ACCEPTED "ifacesAccepted"
#define TAG_IFACES_PROVIDED "ifacesProvided"

#define TAG_COMMON_NAME "name"

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
