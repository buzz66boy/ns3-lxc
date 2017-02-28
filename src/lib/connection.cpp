#include <string>
#include <vector>

#include "iface.h"
#include "connection.h"

using namespace ns3lxc;

Connection::Connection(std::string name, std::string type, int numIfaces){
	_name = name;
	_type = type;
	_numIfaces = numIfaces;
}

int Connection::connectIface(std::string ifaceName, Iface *iface){
	if(ifaces.size() < numIfaces){
		ifaces.push_back(iface);
		return 1;
	}
	return 0; //none added
}