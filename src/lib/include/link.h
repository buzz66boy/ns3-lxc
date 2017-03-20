#ifndef __CONNECTION_H_INCLUDED__
#define __CONNECTION_H_INCLUDED__

// forward declared dependencies


// include dependencies
#include <string>
#include <vector>
#include <memory>

#include "iface.h"

namespace ns3lxc {

// declarations
class Link : public IfaceAccepter {
private:
	std::string name;
	std::string type;
	
	int numIfaces;
	std::vector<std::shared_ptr<Iface> > ifaces;
	
public:
	Link(std::string name, std::string type, int numIfaces): name(name), type(type), numIfaces(numIfaces) {};
	
	std::string getType() { return type; }
	std::string getName() { return name; }
	const std::vector<std::shared_ptr<Iface> >& getIfaces() const { return ifaces; } 
	
	int connectIface(std::string ifaceName, Iface *iface); // OVERRIDES IfaceAccepter
};

}
#endif