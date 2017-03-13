#ifndef __NETWORK_H_INCLUDED__
#define __NETWORK_H_INCLUDED__

// forward declared dependencies

// include dependencies
#include <string>
#include <vector>
#include <cstdint>

#include "position.h"
#include "iface.h"
#include "application.h"
#include "node.h"
#include "connection.h"
#include "ipaddr.h"

namespace ns3lxc {

// declarations
class Topology : public Positionable, public IfaceProvider, public IfaceAccepter {
public:
	std::string name;
	IpAddr *ip;
	IpAddr *subnetMask;

	std::vector<Topology> subtopologies;
	std::vector<Node> nodes;
	std::vector<Connection> connections;
	std::vector<Application> applications;
	
	std::map<std::string, Topology*> topMap;
	std::map<std::string, Node*> nodeMap;
	std::map<std::string, Connection*> connectionMap;
	
	Topology();
};

}

#endif