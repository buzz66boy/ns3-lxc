#include <iostream>
#include <memory>
#include <map>
#include <iostream>
#include "node.h"

using namespace ns3lxc;

static void copyIfaces(const ns3lxc::Node *from, ns3lxc::Node *to){
    to->ifaces = from->ifaces;
    for(auto it = from->ifaces.begin(); it != from->ifaces.end(); it++){
        it->second->node = to;
    }
}

ns3lxc::Node::Node(ns3lxc::Node temp, std::string nodeName): Positionable(), IfaceProvider() {
    name = nodeName;
    copyIfaces(&temp, this);
    applications = temp.applications;

}

ns3lxc::Node::Node(const ns3lxc::Node &temp): Positionable(), IfaceProvider() {
    name = temp.name;
    copyIfaces(&temp, this);
    applications = temp.applications;
}

std::weak_ptr<Iface> ns3lxc::Node::getIface(std::string ifaceName){
    if(ifaces.count(ifaceName) > 0){
        return std::weak_ptr<Iface>(ifaces[ifaceName]);
    } else {
        return std::weak_ptr<Iface>();
    }
}