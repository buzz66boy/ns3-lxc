#ifndef __NAMEABLE_H_INCLUDED__
#define __NAMEABLE_H_INCLUDED__

// forward declared dependencies

// include dependencies
#include <string>
#include <memory>

namespace ns3lxc {

class Nameable {
public:
    std::string name;
    std::string origName;
    Nameable() {}
    Nameable(std::string name): origName(name), name(name) {}
    Nameable(std::string name, std::string origName): name(name), origName(origName) {}
    Nameable(const Nameable& temp): origName(temp.origName), name(temp.name) {}

};

}

#endif