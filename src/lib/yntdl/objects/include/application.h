#ifndef __APPLICATION_H_INCLUDED__
#define __APPLICATION_H_INCLUDED__

// forward declared dependencies
class Node;

// include dependencies
#include <map>
#include <string>
#include <map>

#include "additionalTags.h"
#include "nameable.h"

namespace ns3lxc {

// declarations
class Application: public Nameable, public AdditionalTags {
public:
	std::string path = "";

    /**
     * Map of string command and boolean indicating inheritance
     **/
    std::vector<std::pair<std::string, bool> > commands;
    
    /**
     * Should App be used when templated (user by higher level top)
     **/
    bool inherit = true;

    Application(std::string name, bool inherit = true): inherit(inherit), Nameable(name), AdditionalTags() {};
    Application(std::string name, std::string cmd): Nameable(name), commands({std::pair<std::string, bool> (cmd, false)}) {}
    Application(const Application& temp);

    /** 
     * Constructor to filter out non-inherited commands
     **/
    Application(Application *temp);

    void addCommand(std::string cmd, bool shouldInherit = true) {commands.push_back(std::pair<std::string, bool> (cmd, shouldInherit)); }
};

}

#endif