#include <iostream>
#include <memory>
#include <map>
#include <vector>
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
    nodeNum = temp.nodeNum;
    for(auto cmdPair : temp.commands){
        if(cmdPair.second){
            commands.push_back(cmdPair);
        }
        // else {
        //     std::cout << "Filtering command " + cmdPair.first + " for node " + name << std::endl;
        // }
    }
    for(Application app : temp.applications){
        if(app.inherit){
            applications.push_back(Application(&app));
        }
        // else {
            // std::cout << "Filtering out app " + app.name + " on node " + name << std::endl;
        // }
    }
}

ns3lxc::Node::Node(const ns3lxc::Node &temp): Positionable(temp), Nameable(temp), AdditionalTags(temp), IfaceProvider() {
    ifaces = temp.ifaces;
    reRefIfaces(this);
    requiresReRef = true;
    type = temp.type;
    applications = temp.applications;
    nodeNum = temp.nodeNum;
    commands = temp.commands;
}

void ns3lxc::Node::addCommand(std::string cmd, bool inherit){
    commands.push_back(std::pair<std::string, bool>(cmd, inherit));
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