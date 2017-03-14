#ifndef __TOPOLOGY_PARSER_H_INCLUDED__
#define __TOPOLOGY_PARSER_H_INCLUDED__

// forward declared dependencies

// include dependencies
#include <string>
#include <vector>
#include <map>

#include <arpa/inet.h>

#include "topology.h"
#include "node.h"
#include "application.h"

// declarations

// NOTE: these tags can also be used plurally (checks for tag + "s")
#define TAG_INCLUDE "include" 
#define TAG_NODE "node" 
#define TAG_TOPOLOGY "topology"
#define TAG_APPLICATION "application"
#define TAG_CONNECTION "link"
#define TAG_POSITION "position"
#define TAG_IFACE "iface"
#define TAG_TEMPLATE "template"
#define TAG_NUM "num"

#define TAG_IFACES_ACCEPTED "ifacesAccepted"
#define TAG_IFACES_PROVIDED "ifacesProvided"

#define TAG_NAME "name"

#define TAG_INTERFACE_IP "ip"

#define TAG_NETWORK_TYPE "type"
#define TAG_NETWORK_CONNECTED "connected"

class ParsedTopology {
public:
    ns3lxc::Topology topology;
	std::map<std::string, ns3lxc::Topology *> includedTopologies;
	std::map<std::string, ns3lxc::Node *> nodes;
	std::map<std::string, ns3lxc::Link *> links;
	std::map<std::string, ns3lxc::Application *> applications;
};

std::string pluralize(std::string str);

ns3lxc::Topology parseTopologyFile(std::string topPath);

#endif
