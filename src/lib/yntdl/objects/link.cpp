#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include "link.h"

using namespace yntdl;

Link::Link(const Link& ln): Nameable(ln), AdditionalTags(ln) {
    type = ln.type;
    if(ln.ip){
        ip = new IpAddr(*ln.ip);
    }
    if(ln.subnetMask){
        subnetMask = new IpAddr(*ln.subnetMask);
    }
    bandwidth = ln.bandwidth;
    latency = ln.latency;
    ifaces.assign(ln.ifaces.begin(), ln.ifaces.end());
    for(auto iface : ifaces){
        iface->link = this;
    }
}

Link::Link(std::string name, yntdl::Link& ln): Nameable(name), AdditionalTags(ln) {
    type = ln.type;
    bandwidth = ln.bandwidth;
    latency = ln.latency;
    if(ln.ip){
        ip = new IpAddr(*ln.ip);
    }
    if(ln.subnetMask){
        subnetMask = new IpAddr(*ln.subnetMask);
    }
}

Link::~Link(){
    if(ip){
        delete ip;
    }
    if(subnetMask){
        delete subnetMask;
    }
}

int Link::connectIface(std::string ifaceName, yntdl::Iface *iface){
	return connectIface(iface);
}

int Link::connectIface(yntdl::Iface *iface){
        ifaces.push_back(iface);
        if(subnetMask){
            iface->assignSubnetMask(subnetMask);
        }
        iface->link = this;
        return 1;
}

void Link::reRefIfaces(Link *linkPtr){
    for(auto i = 0; i < linkPtr->ifaces.size(); ++i){
        linkPtr->ifaces[i]->link = linkPtr;
    }
}