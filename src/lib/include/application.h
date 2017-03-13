#ifndef __APPLICATION_H_INCLUDED__
#define __APPLICATION_H_INCLUDED__

// forward declared dependencies
class Node;

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
	std::map<Node *, std::string> argMap;
};

}

#endif