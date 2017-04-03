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
    address = ifa.address;
}

std::weak_ptr<ns3lxc::Iface> ns3lxc::IfaceProvider::getIface(std::string ifaceName){
    if(ifacesProvided[ifaceName] != nullptr && !ifacesProvidedSubNames[ifaceName].empty()){
        return ifacesProvided[ifaceName]->getIface( ifacesProvidedSubNames[ifaceName] );
    }
    std::cout<< "BADBADBAD" << std::endl;
    return std::weak_ptr<ns3lxc::Iface>();
}

int ns3lxc::IfaceAccepter::connectIface(std::string ifaceName, std::weak_ptr<ns3lxc::Iface> iface){
    if(ifacesAccepted[ifaceName] != nullptr && !ifacesAcceptedSubNames[ifaceName].empty()){
        return ifacesAccepted[ifaceName]->connectIface( ifacesAcceptedSubNames[ifaceName], iface );
    }
    std::cout<< "BADBADBAD 2" << std::endl;
    return 0;
}