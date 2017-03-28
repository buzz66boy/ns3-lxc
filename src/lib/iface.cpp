#include <string>
#include <vector>
#include <map>
#include <memory>

#include "iface.h"

using namespace ns3lxc;

std::shared_ptr<ns3lxc::Iface> ns3lxc::IfaceProvider::getIface(std::string ifaceName){
    if(ifacesProvided[ifaceName] != nullptr && !ifacesProvidedSubNames[ifaceName].empty()){
        return ifacesProvided[ifaceName]->getIface( ifacesProvidedSubNames[ifaceName] );
    }
    return nullptr;
}

int ns3lxc::IfaceAccepter::connectIface(std::string ifaceName, std::shared_ptr<ns3lxc::Iface> iface){
    if(ifacesAccepted[ifaceName] != nullptr && !ifacesAcceptedSubNames[ifaceName].empty()){
        return ifacesAccepted[ifaceName]->connectIface( ifacesAcceptedSubNames[ifaceName], iface );
    }
    return 0;
}