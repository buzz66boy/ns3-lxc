#include <string>
#include <vector>

#include "connection.h"

using namespace ns3lxc;

int ns3lxc::Connection::connectIface(std::string ifaceName, Iface *iface){
	if(ifaces.size() < numIfaces){
		ifaces.push_back(iface);
		return 1;
	}
	return 0; //none added
}