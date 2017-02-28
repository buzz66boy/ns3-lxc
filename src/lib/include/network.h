#ifndef __NETWORK_H_INCLUDED__
#define __NETWORK_H_INCLUDED__

// forward declared dependencies
class Container;
class Application;
class Connection;

// include dependencies
#include <string>
#include <vector>
#include <cstdint>

#include "position.h"
#include "ifaceHelper.h"
#include "ipaddr.h"

namespace ns3lxc {

// declarations
class Network : public Positionable, public IfaceProvider, public IfaceAccepter {
public:
	std::string name;
	IpAddr ip;
	IpAddr subnetMask;
	
	std::vector<Network> subnetworks;
	std::vector<Container> containers;
	std::vector<Connection> connections;
	std::vector<Application> applications;
	
	std::map<std::string, Network*> netMap;
	std::map<std::string, Container*> containerMap;
	std::map<std::string, Connection*> connectionMap;
	
};

}

#endif