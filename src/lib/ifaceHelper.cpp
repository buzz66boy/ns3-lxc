#include <string>
#include <vector>
#include <map>

#include "ifaceHelper.h"

using namespace ns3lxc;

Iface *IfaceProvider::getIface(std::string ifaceName){
	if(ifacesProvided[ifaceName] != nullptr && !ifacesProvidedSubNames[ifaceName].empty()){
		return ifacesProvided[ifaceName]->getIface( ifacesProvidedSubNames[ifaceName] );
	}
	return nullptr;
}

int IfaceAccepter::connectIface(std::string ifaceName, Iface *iface){
	if(ifacesAccepted[ifaceName] != nullptr && !ifacesAcceptedSubNames[ifaceName].empty()){
		return ifacesAccepted[ifaceName]->connectIface( ifacesAcceptedSubNames[ifaceName], iface );
	}
	return 0;
}