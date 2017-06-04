#include <iostream>
#include <memory>
#include <map>
#include <iostream>
#include <algorithm>

#include "errorCode.h"
#include "link.h"
#include "node.h"

using namespace ns3lxc;

void ns3lxc::Node::reRefIfaces(ns3lxc::Node *node){
    for(auto it = node->ifaces.begin(); it != node->ifaces.end(); it++){
        it->second.node = node;
    }
}

ns3lxc::Node::Node(ns3lxc::Node& temp, std::string nodeName, std::string origName): Positionable(temp), Nameable(nodeName, origName), AdditionalTags(temp), IfaceProvider() {
    ifaces = temp.ifaces;
    reRefIfaces(this);
    requiresReRef = true;
    type = temp.type;
    applications = temp.applications;
    nodeNum = temp.nodeNum;
}

ns3lxc::Node::Node(const ns3lxc::Node &temp): Positionable(temp), Nameable(temp), AdditionalTags(temp), IfaceProvider() {
    ifaces = temp.ifaces;
    reRefIfaces(this);
    requiresReRef = true;
    type = temp.type;
    applications = temp.applications;
    nodeNum = temp.nodeNum;
}

Iface *ns3lxc::Node::getIface(std::string ifaceName){
    if(requiresReRef){
        reRefIfaces(this);
        requiresReRef = false;
    }
    if(ifaces.count(ifaceName) > 0){
        return &ifaces[ifaceName];
    }
    throw Ns3lxcException(ErrorCode::IFACE_NOT_FOUND, ifaceName + " on Node " + name);
}