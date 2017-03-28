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
	
	int numIfaces = 0;
	std::vector<std::shared_ptr<Iface> > ifaces;
	
public:
	Link(std::string name, std::string type, int numIfaces): name(name), type(type), numIfaces(numIfaces) {};
	Link(std::string name, Link link);
	Link(std::string name): name(name) {}

	std::string getType() { return type; }
	void setType(std::string typ) { type = typ; }

	std::string getName() { return name; }

	int getNumIface() { return numIfaces; }
	void setNumIface(int num) { numIfaces = num; }

	const std::vector<std::shared_ptr<Iface> >& getIfaces() const { return ifaces; } 
	
	int connectIface(std::shared_ptr<ns3lxc::Iface> iface); // OVERRIDES IfaceAccepter
};

}
#endif