#ifndef __IFACE_H_INCLUDED__
#define __IFACE_H_INCLUDED__

// forward declared dependencies
class Container;
class Connection;
class IpAddr;

// include dependencies
#include <string>
#include "ipaddr.h"
#include "ifaceHelper.h"

namespace ns3lxc {

// declarations

class Iface : public IfaceProvider {
public:
	std::string name;
	Container *container;
	Connection *connection;
	IpAddr address;
	Iface *getIface(std::string ifaceName) { return this; }
};

}

#endif