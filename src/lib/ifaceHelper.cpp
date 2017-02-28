#include <string>
#include <vector>
#include <map>

#include "ifaceHelper.h"
#include "iface.h"

using namespace ns3lxc;

Iface *IfaceProvider::getIface(std::string ifaceName){
	if(ifacesProvided != NULL && ifacesProvidedSubNames != NULL){
		if(ifacesProvided[ifaceName] != nullptr && ifacesProvidedSubNames[ifaceName] != NULL){
			return ifacesProvided[ifaceName]->getIface( ifacesProvidedSubNames[ifaceName] );
		}
	}
	return nullptr;
}

int IfaceAccepter::connectIface(std::string ifaceName, Iface *iface){
	if(ifacesAccepted != NULL && ifacesAcceptedSubNames != NULL){
		if(ifacesAccepted[ifaceName] != nullptr && ifacesAcceptedSubNames[ifaceName] != NULL){
			return ifacesAccepted[ifaceName]->connectIface( ifacesAcceptedSubNames[ifaceName], iface );
		}
	}
	return 0;
}