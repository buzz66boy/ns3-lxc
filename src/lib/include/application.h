#ifndef __APPLICATION_H_INCLUDED__
#define __APPLICATION_H_INCLUDED__

// forward declared dependencies
class Container;

// include dependencies
#include <vector>
#include <string>
#include <map>

namespace ns3lxc {

// declarations
class Application {
public:
	std::string name;
	std::string path;
	std::map<Container *, std::string> argMap;
};

}

#endif