#ifndef __APPLICATION_H_INCLUDED__
#define __APPLICATION_H_INCLUDED__

// forward declared dependencies
class Node;

// include dependencies
#include <vector>
#include <string>
#include <map>

#include "nameable.h"

namespace ns3lxc {

// declarations
class Application: public Nameable {
public:
	std::string path;
	std::map<std::string, std::string> argMap;

    Application(std::string name): Nameable(name) {};
    Application(const Application& temp);
};

}

#endif