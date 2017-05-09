#include <string>
#include <memory>
#include <iostream>

#include "topology.h"

using namespace ns3lxc;

Topology::Topology(){
    
}

Topology::Topology(std::shared_ptr<Topology> temp, std::string newName): Topology(temp.get()){
    name = newName;
}

Topology::Topology(Topology *temp){
    name = temp->name;

    size_t i;
    for(i = 0; i < temp->subTopologies.size(); ++i){
        std::shared_ptr<Topology> ptr = std::shared_ptr<Topology>(new Topology(temp->subTopologies[i].get()));
        subTopologies.push_back(ptr);
        topMap[ptr->name] = ptr;
    }
    for(i = 0; i < temp->links.size(); ++i){
        std::shared_ptr<Link> ptr = std::shared_ptr<Link>(new Link(*temp->links[i]));
        ptr->ifaces.clear();
        links.push_back(ptr);
        linkMap[ptr->name] = ptr;
    }
    for(i = 0; i < temp->nodes.size(); ++i){
        std::shared_ptr<Node> ptr = std::shared_ptr<Node>(new Node(*temp->nodes[i], temp->nodes[i]->name));
        nodes.push_back(ptr);
        nodeMap[ptr->name] = ptr;
        Node::reRefIfaces(ptr.get());
        for(auto ifacePair : ptr->ifaces){
            if(ifacePair.second->ip != nullptr){
                ifacePair.second->ip = new ns3lxc::IpAddr(*ifacePair.second->ip);
            }
            if(ifacePair.second->link != nullptr){
                ifacePair.second->link = linkMap[ifacePair.second->link->name].get();
                ifacePair.second->link->connectIface(ifacePair.second);
                Link::reRefIfaces(ifacePair.second->link);
                std::cout << ifacePair.second->node->name << ": " << ptr->name << std::endl;
            }
        }
    }
    for(i = 0; i < temp->applications.size(); ++i){
        std::shared_ptr<Application> ptr = std::shared_ptr<Application>(new Application(*temp->applications[i]));
        applications.push_back(ptr);
    }
    ifacesProvidedSubNames.insert(temp->ifacesProvidedSubNames.begin(), temp->ifacesProvidedSubNames.end());
    for(auto it: temp->ifacesProvided){
        try {
            Node nodeCast = dynamic_cast<Node&>(*it.second.lock());
            if(nodeMap.count(nodeCast.name) > 0){
                ifacesProvided[it.first] = nodeMap[nodeCast.name];
            } else {
                std::cerr << "ISSUES 1" << std::endl;
            }
        } catch (std::bad_cast e){
            try{
                Topology topCast = dynamic_cast<Topology&>(*it.second.lock());
                if (topMap.count(topCast.name) > 0){
                    ifacesProvided[it.first] = topMap[topCast.name];
                } else {
                    std::cerr << "ISSUES 2" << std::endl;
                }
            } catch (std::bad_cast e){
                std::cerr << "BAD CAST 2" << std::endl;
            }
        }
    }
    curNodeNum = temp->curNodeNum;
}

Topology::Topology(const Topology& temp){
    name = temp.name;

    size_t i;
    runTime = temp.runTime;
    subTopologies = temp.subTopologies;
    nodes = temp.nodes;
    links = temp.links;
    applications = temp.applications;
    topMap = temp.topMap;
    nodeMap = temp.nodeMap;
    linkMap = temp.linkMap;
    ifacesProvided = temp.ifacesProvided;
    ifacesProvidedSubNames = temp.ifacesProvidedSubNames;
    curNodeNum = temp.curNodeNum;
}

static int reNum(Topology *top, int curNum){
    for(auto topPtr : top->subTopologies){
        curNum = reNum(topPtr.get(), curNum);
    }
    for(auto nodePtr : top->nodes){
        nodePtr->nodeNum = curNum++;
    }
    top->curNodeNum = curNum;
    return curNum;
}

void Topology::reNumNodes(Topology *top){
    top->curNodeNum = 0;
    reNum(top, 0);

}