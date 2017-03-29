#include <iostream>
#include <memory>
#include <map>
#include "node.h"

using namespace ns3lxc;

ns3lxc::Node::Node(ns3lxc::Node temp, std::string nodeName): Positionable(), IfaceProvider() {
    name = nodeName;

}

ns3lxc::Node::Node(const ns3lxc::Node &temp): Positionable(), IfaceProvider() {
    name = temp.name;

}

std::shared_ptr<Iface> ns3lxc::Node::getIface(std::string ifaceName){
    if(ifaces.count(ifaceName) > 0){
        return ifaces[ifaceName];
    } else {
        return nullptr;
    }
}