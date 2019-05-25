#ifndef __LINK_H_INCLUDED__
#define __LINK_H_INCLUDED__

// forward declared dependencies


// include dependencies
#include <string>
#include <vector>
#include <memory>

#include "additionalTags.h"
#include "iface.h"
#include "nameable.h"

namespace yntdl {

// declarations
class Link : public IfaceAcceptor, public Nameable, public AdditionalTags {
private:
	bool requiresReRef = false;
	std::string type;
	
public:

	std::vector<Iface *> ifaces;

	IpAddr *ip = nullptr;
	IpAddr *subnetMask = nullptr;

	std::string bandwidth;
	std::string latency;
	
	Link(std::string name, std::string type): Nameable(name), AdditionalTags(), type(type) {};
	Link(std::string name, Link& link);
	Link(std::string name): Nameable(name) {}
	Link(const Link&);
	~Link();

	std::string getType() { return type; }
	void setType(std::string typ) { type = typ; }

	const std::vector<Iface*>& getIfaces() const { return ifaces; } 
	
	int connectIface(yntdl::Iface *iface); 
	int connectIface(std::string ifaceName, yntdl::Iface *iface) override; // OVERRIDES IfaceAccepter

	static void reRefIfaces(Link *linkPtr);
};

}
#endif