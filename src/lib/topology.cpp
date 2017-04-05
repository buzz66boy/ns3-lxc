#include <string>
#include <memory>

#include "topology.h"

using namespace ns3lxc;

Topology::Topology(){
    
}

Topology::Topology(Topology *temp, std::string name): name(name){
    Topology((const Topology&) *temp);
}

Topology::Topology(const Topology& temp){
    size_t i;
    for(i = 0; i < temp.subTopologies.size(); ++i){
        std::shared_ptr<Topology> ptr = std::shared_ptr<Topology>(new Topology(*temp.subTopologies[i]));
        subTopologies.push_back(ptr);
        topMap[ptr->name] = ptr;
    }
    for(i = 0; i < temp.nodes.size(); ++i){
        std::shared_ptr<Node> ptr = std::shared_ptr<Node>(new Node(*temp.nodes[i]));
        nodes.push_back(ptr);
        nodeMap[ptr->name] = ptr;
    }
    for(i = 0; i < temp.links.size(); ++i){
        std::shared_ptr<Link> ptr = std::shared_ptr<Link>(new Link(*temp.links[i]));
        links.push_back(ptr);
        linkMap[ptr->getName()] = ptr;
    }
    for(i = 0; i < temp.applications.size(); ++i){
        std::shared_ptr<Application> ptr = std::shared_ptr<Application>(new Application(*temp.applications[i]));
        applications.push_back(ptr);
    }
}