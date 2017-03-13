#ifndef __CONTAINER_H_INCLUDED__
#define __CONTAINER_H_INCLUDED__

// forward declared dependencies
class Application;
class Iface;

// include dependencies
#include <string>
#include <map>
#include <vector>

#include "position.h"
#include "iface.h"

namespace ns3lxc {

// declarations
class Node : public Positionable, public IfaceProvider {
public:
	std::string name;
	std::map<std::string, Iface *> ifaces;
	std::vector<Application> applications;
	
	Iface *getIface(std::string ifaceName) { return ifaces[ifaceName]; } // OVERRIDE IfaceProvider
};

}
#endif