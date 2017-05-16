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
	std::string args;

    Application(std::string name): Nameable(name) {};
    Application(std::string name, std::string args): Nameable(name), args(args) {};
    Application(const Application& temp);
};

}

#endif