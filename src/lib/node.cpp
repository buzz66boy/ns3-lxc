#include "node.h"

using namespace ns3lxc;



ns3lxc::Node::Node(ns3lxc::Node temp, std::string nodeName){
    name = nodeName;

}

ns3lxc::Node::Node(const ns3lxc::Node &temp){
    name = temp.name;

}