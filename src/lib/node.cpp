#include <iostream>
#include <memory>
#include <map>
#include <iostream>
#include <algorithm>

#include "link.h"
#include "node.h"

using namespace ns3lxc;

void ns3lxc::Node::reRefIfaces(ns3lxc::Node *node){
    for(auto it = node->ifaces.begin(); it != node->ifaces.end(); it++){
        it->second->node = node;
        if(it->second->link != nullptr){
            Link *link = it->second->link;
            auto iter = std::find(link->ifaces.begin(), link->ifaces.end(), it->second);
            if(iter != link->ifaces.end()){
                link->ifaces.erase(iter);
            } else {
                std::cerr << "DBDBDBDB" << std::endl;
            }
            link->ifaces.push_back(it->second);
        } else {
            // std::cerr << "NULL LINK FOR " << it->first << std::endl;
        }
    }
}

ns3lxc::Node::Node(ns3lxc::Node& temp, std::string nodeName): Positionable(), IfaceProvider() {
    name = nodeName;
    for(auto it = temp.ifaces.begin(); it != temp.ifaces.end(); it++){
        ifaces[it->first] = std::shared_ptr<Iface>(new Iface(*it->second));
        ifaces[it->first]->link = nullptr;
        ifaces[it->first]->node = nullptr;
    }
    requiresReRef = true;
    applications = temp.applications;
}

ns3lxc::Node::Node(const ns3lxc::Node &temp): Positionable(), IfaceProvider() {
    name = temp.name;
    ifaces = temp.ifaces;
    for(auto ifacePair : ifaces){
        ifacePair.second->node = this;

    }
    requiresReRef = true;
    applications = temp.applications;
    bridges = temp.bridges;
    taps = temp.taps;
    nodeNum = temp.nodeNum;
}

std::weak_ptr<Iface> ns3lxc::Node::getIface(std::string ifaceName){
    if(requiresReRef){
        reRefIfaces(this);
        requiresReRef = false;
    }
    if(ifaces.count(ifaceName) > 0){
        return std::weak_ptr<Iface>(ifaces[ifaceName]);
    } else {
        std::cerr << "Couldn't find iface " << ifaceName << " " << ifaces.size() << std::endl;
        return std::weak_ptr<Iface>();
    }
}