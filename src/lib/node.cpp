#include <iostream>
#include <memory>
#include <map>
#include <iostream>
#include "node.h"

using namespace ns3lxc;

static void copyIfaces(const ns3lxc::Node *from, const ns3lxc::Node *to){
    for(auto it = from->ifaces.begin(); it != from->ifaces.end(); it++){

    }
}

ns3lxc::Node::Node(ns3lxc::Node temp, std::string nodeName): Positionable(), IfaceProvider() {
    name = nodeName;

}

ns3lxc::Node::Node(const ns3lxc::Node &temp): Positionable(), IfaceProvider() {
    name = temp.name;
    ifaces = temp.ifaces;
    applications = temp.applications;
    std::cout << "copy node" << std::endl;
}

std::weak_ptr<Iface> ns3lxc::Node::getIface(std::string ifaceName){
    if(ifaces.count(ifaceName) > 0){
        return std::weak_ptr<Iface>(ifaces[ifaceName]);
    } else {
        return std::weak_ptr<Iface>();
    }
}