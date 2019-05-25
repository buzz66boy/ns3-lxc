#ifndef __NODE_H_INCLUDED__
#define __NODE_H_INCLUDED__

// forward declared dependencies

// include dependencies
#include <string>
#include <map>
#include <vector>
#include <memory>

#include "application.h"
#include "position.h"
#include "iface.h"
#include "nameable.h"
#include "additionalTags.h"

namespace yntdl {

// declarations
class Node : public Positionable, public IfaceProvider, public Nameable, public AdditionalTags {
private:
    bool requiresReRef = false;
public:
    int nodeNum = -1; //For ns-3 writing, keep track of what # the node is (for ref)
    std::string type;
	std::map<std::string, Iface> ifaces;
	std::vector<Application> applications;
    std::vector<std::pair<std::string, bool> > commands;
	
	yntdl::Iface *getIface(std::string ifaceName) override; // OVERRIDE IfaceProvider

    Node(): Nameable(), Positionable(), IfaceProvider(), AdditionalTags() {};
    Node(std::string name):  Nameable(name), Positionable(), IfaceProvider() {};
    Node(yntdl::Node& temp, std::string nodeName, std::string origName);
    Node(const yntdl::Node &temp);

    void addCommand(std::string cmd, bool inherit);

    static void reRefIfaces(yntdl::Node*);
};

}
#endif