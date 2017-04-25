#include <iostream>
#include <memory>
#include <map>
#include <iostream>
#include "node.h"

using namespace ns3lxc;

static void copyIfaces(const ns3lxc::Node *from, ns3lxc::Node *to){
    //FIXME: refs don't copy right
    for(auto it = from->ifaces.begin(); it != from->ifaces.end(); it++){
        to->ifaces[it->first] = std::shared_ptr<Iface>(new Iface(*it->second));
    }
}

void ns3lxc::Node::reRefIfaces(ns3lxc::Node *node){
    for(auto it = node->ifaces.begin(); it != node->ifaces.end(); it++){
        it->second->node = node;
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
    bridges = temp.bridges;
    taps = temp.taps;
    nodeNum = temp.nodeNum;
}

std::weak_ptr<Iface> ns3lxc::Node::getIface(std::string ifaceName){
    if(ifaces.count(ifaceName) > 0){
        return std::weak_ptr<Iface>(ifaces[ifaceName]);
    } else {
        return std::weak_ptr<Iface>();
    }
}