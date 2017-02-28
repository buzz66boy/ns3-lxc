#ifndef __IFACEHELPER_H_INCLUDED__
#define __IFACEHELPER_H_INCLUDED__

// forward declared dependencies
class Iface;

// include dependencies
#include <string>
#include <map>

namespace ns3lxc {

// declarations
class IfaceProvider {
public:
	std::map<std::string, std::string> ifacesProvidedSubNames; //maps this level's iface names to lower level's
	std::map<std::string, IfaceProvider *> ifacesProvided; //keep a ref to the providers we contain
	Iface *getIface(std::string ifaceName);
};

class IfaceAccepter {
public:
	std::map<std::string, std::string> ifacesAcceptedSubNames; //maps this level's iface names to lower level's
	std::map<std::string, IfaceAccepter *> ifacesAccepted; //keep a ref to who below us accepts ifaces
	int connectIface(std::string ifaceName, Iface *iface);
};

}

#endif