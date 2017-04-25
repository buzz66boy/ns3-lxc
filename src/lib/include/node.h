#ifndef __CONTAINER_H_INCLUDED__
#define __CONTAINER_H_INCLUDED__

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

namespace ns3lxc {

// declarations
class Node : public Positionable, public IfaceProvider, public Nameable {
public:
    int nodeNum = 0; //For ns-3 writing, keep track of what # the node is (for ref)

	std::map<std::string, std::shared_ptr<Iface> > ifaces;
	std::vector<Application> applications;
    std::vector<std::string> bridges;
    std::vector<std::string> taps;
	
	std::weak_ptr<ns3lxc::Iface> getIface(std::string ifaceName) override; // OVERRIDE IfaceProvider

    Node(): Positionable(), IfaceProvider() {};
    Node(std::string name):  Nameable(name), Positionable(), IfaceProvider() {};
    Node(ns3lxc::Node temp, std::string nodeName);
    Node(const ns3lxc::Node &temp);

    static void reRefIfaces(ns3lxc::Node*);
};

}
#endif