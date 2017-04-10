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
    Nameable() {}
    Nameable(std::string name): name(name) {}

};

}

#endif