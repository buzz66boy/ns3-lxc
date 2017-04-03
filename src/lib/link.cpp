#include <string>
#include <vector>
#include <memory>

#include "link.h"

using namespace ns3lxc;

ns3lxc::Link::Link(const Link& ln){
    type = ln.type;
    name = ln.name;
    numIfaces = ln.numIfaces;
    ifaces = ln.ifaces;
}

ns3lxc::Link::Link(std::string name, ns3lxc::Link link): name(name) {
    type = link.type;
}

int ns3lxc::Link::connectIface(std::string ifaceName, std::weak_ptr<ns3lxc::Iface> iface){
	connectIface(iface);
}

int ns3lxc::Link::connectIface(std::weak_ptr<ns3lxc::Iface> iface){
    if(numIfaces == 0 || ifaces.size() < numIfaces){
        ifaces.push_back(iface.lock());
        return 1;
    }
    return 0; //none added
}