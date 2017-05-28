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

	std::vector<Iface *> ifaces;

	IpAddr *ip = nullptr;
	IpAddr *subnetMask = nullptr;

	std::string bandwidth;
	std::string latency;
	
	Link(std::string name, std::string type, int numIfaces): Nameable(name), type(type), numIfaces(numIfaces) {};
	Link(std::string name, Link& link);
	Link(std::string name): Nameable(name) {}
	Link(const Link&);
	~Link();

	std::string getType() { return type; }
	void setType(std::string typ) { type = typ; }

	int getNumIface() { return numIfaces; }
	void setNumIface(int num) { numIfaces = num; }

	const std::vector<Iface*>& getIfaces() const { return ifaces; } 
	
	int connectIface(ns3lxc::Iface *iface); 
	int connectIface(std::string ifaceName, ns3lxc::Iface *iface) override; // OVERRIDES IfaceAccepter

	static void reRefIfaces(Link *linkPtr);
};

}
#endif