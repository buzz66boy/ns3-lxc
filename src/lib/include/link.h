#ifndef __CONNECTION_H_INCLUDED__
#define __CONNECTION_H_INCLUDED__

// forward declared dependencies


// include dependencies
#include <string>
#include <vector>
#include <memory>

#include "iface.h"
#include "nameable.h"

namespace ns3lxc {

// declarations
class Link : public IfaceAcceptor, public Nameable {
private:
	bool requiresReRef = false;
	
	std::string type;
	
	int numIfaces = 0;
	
public:

	std::vector<std::shared_ptr<Iface> > ifaces;

	IpAddr *ip = nullptr;
	IpAddr *subnetMask = nullptr;
	
	Link(std::string name, std::string type, int numIfaces): Nameable(name), type(type), numIfaces(numIfaces) {};
	Link(std::string name, Link& link);
	Link(std::string name): Nameable(name) {}
	Link(const Link&);
	~Link();

	std::string getType() { return type; }
	void setType(std::string typ) { type = typ; }

	int getNumIface() { return numIfaces; }
	void setNumIface(int num) { numIfaces = num; }

	const std::vector<std::shared_ptr<Iface> >& getIfaces() const { return ifaces; } 
	
	int connectIface(std::weak_ptr<ns3lxc::Iface> iface); 
	int connectIface(std::string ifaceName, std::weak_ptr<ns3lxc::Iface> iface); // OVERRIDES IfaceAccepter

	static void reRefIfaces(Link *linkPtr);
};

}
#endif