#ifndef __IPADDR_H_INCLUDED__
#define __IPADDR_H_INCLUDED__

// include dependencies
#include <string>
#include <vector>
#include <map>

#include <arpa/inet.h>

class Ipaddr {
public:
	std::string str();
};

#endif