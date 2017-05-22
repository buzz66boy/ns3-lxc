#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include "link.h"

using namespace ns3lxc;

Link::Link(const Link& ln): Nameable(ln){
    type = ln.type;
    numIfaces = ln.numIfaces;
    if(ln.ip){
        ip = new IpAddr(*ln.ip);
    }
    if(ln.subnetMask){
        subnetMask = new IpAddr(*ln.subnetMask);
    }
    ifaces.assign(ln.ifaces.begin(), ln.ifaces.end());
    for(auto iface : ifaces){
        iface->link = this;
    }
}

Link::Link(std::string name, ns3lxc::Link& link): Nameable(name) {
    type = link.type;
}

Link::~Link(){
    if(ip){
        delete ip;
    }
    if(subnetMask){
        delete subnetMask;
    }
}

int Link::connectIface(std::string ifaceName, ns3lxc::Iface *iface){
	connectIface(iface);
}

int Link::connectIface(ns3lxc::Iface *iface){
    if(numIfaces == 0 || ifaces.size() < numIfaces){
        ifaces.push_back(iface);
        iface->link = this;
        return 1;
    }
    return 0; //none added
}

void Link::reRefIfaces(Link *linkPtr){
    for(auto i = 0; i < linkPtr->ifaces.size(); ++i){
        linkPtr->ifaces[i]->link = linkPtr;
    }
}