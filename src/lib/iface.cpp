#include <string>
#include <vector>
#include <map>
#include <memory>
#include <iostream>

#include "iface.h"

using namespace ns3lxc;

ns3lxc::Iface::Iface(const Iface& ifa){
    name = ifa.name;
    node = ifa.node;
    link = ifa.link;
    ip = ifa.ip;
    subnetMask = ifa.subnetMask;
    tapName = ifa.tapName;
    bridgeName = ifa.bridgeName;
    macAddr = ifa.macAddr;
}

std::weak_ptr<ns3lxc::Iface> ns3lxc::IfaceProvider::getIface(std::string ifaceName){
    if(ifacesProvided.count(ifaceName) > 0){
        std::cout <<"dasssGUD" <<std::endl;
        return ifacesProvided[ifaceName].lock()->getIface( ifacesProvidedSubNames[ifaceName] );
    }
    return std::weak_ptr<ns3lxc::Iface>();
}

int ns3lxc::IfaceAccepter::connectIface(std::string ifaceName, std::weak_ptr<ns3lxc::Iface> iface){
    if(ifacesAccepted.count(ifaceName) > 0){
        return ifacesAccepted[ifaceName].lock()->connectIface( ifacesAcceptedSubNames[ifaceName], iface );
    }
    return 0;
}