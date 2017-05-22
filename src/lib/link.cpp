#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include "link.h"

using namespace ns3lxc;

ns3lxc::Link::Link(const Link& ln): Nameable(ln){
    type = ln.type;
    numIfaces = ln.numIfaces;
    ip = ln.ip;
    subnetMask = ln.subnetMask;
    ifaces.assign(ln.ifaces.begin(), ln.ifaces.end());
    for(auto iface : ifaces){
        iface->link = this;
    }
}

ns3lxc::Link::Link(std::string name, ns3lxc::Link& link): Nameable(name) {
    type = link.type;
}

int ns3lxc::Link::connectIface(std::string ifaceName, std::weak_ptr<ns3lxc::Iface> iface){
	connectIface(iface);
}

int ns3lxc::Link::connectIface(std::weak_ptr<ns3lxc::Iface> iface){
    if(numIfaces == 0 || ifaces.size() < numIfaces){
        ifaces.push_back(iface.lock());
        iface.lock()->link = this;
        return 1;
    }
    return 0; //none added
}

void ns3lxc::Link::reRefIfaces(Link *linkPtr){
    for(auto i = 0; i < linkPtr->ifaces.size(); ++i){
        linkPtr->ifaces[i]->link = linkPtr;
    }
}