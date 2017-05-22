#include <string>
#include <memory>
#include <iostream>

#include "topology.h"

using namespace ns3lxc;

Topology::Topology(){
    
}

Topology::Topology(std::shared_ptr<Topology> temp, std::string newName): Topology(temp.get()){
    name = newName;
    origName = newName;
}

static Link *findLink(Topology *top, std::string linkName){
    if(top->linkMap.count(linkName) > 0){
        return top->linkMap.at(linkName).get();
    } else {
        for(auto topPtr : top->subTopologies){
            Link *ln = findLink(topPtr.get(), linkName);
            if(ln != nullptr){
                return ln;
            }
        }
        return nullptr;
    }
}

Topology::Topology(Topology *temp): Nameable(*temp), Positionable(*temp) {
    size_t i;
    for(i = 0; i < temp->subTopologies.size(); ++i){
        std::shared_ptr<Topology> ptr = std::shared_ptr<Topology>(new Topology(temp->subTopologies[i].get()));
        subTopologies.push_back(ptr);
        topMap[ptr->origName] = ptr;
        std::cout << ptr->origName << std::endl;
    }
    for(i = 0; i < temp->links.size(); ++i){
        std::shared_ptr<Link> ptr = std::shared_ptr<Link>(new Link(*temp->links[i]));
        ptr->ifaces = std::vector<std::shared_ptr<Iface> >();
        links.push_back(ptr);
        linkMap[ptr->origName] = ptr;
    }
    for(i = 0; i < temp->nodes.size(); ++i){
        std::shared_ptr<Node> ptr = std::shared_ptr<Node>(new Node(*temp->nodes[i], temp->nodes[i]->origName));
        nodes.push_back(ptr);
        nodeMap[ptr->origName] = ptr;
        Node::reRefIfaces(ptr.get());
        for(auto ifacePair : ptr->ifaces){
            if(ifacePair.second->ip != nullptr){
                ifacePair.second->ip = new ns3lxc::IpAddr(*ifacePair.second->ip);
            }
            if(ifacePair.second->link != nullptr){
                std::string name = ifacePair.second->link->origName;
                if(linkMap.count(name) > 0){
                    ifacePair.second->link = linkMap.at(name).get();
                    ifacePair.second->link->connectIface(ifacePair.second);
                    Link::reRefIfaces(ifacePair.second->link);
                }
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
            if(nodeMap.count(nodeCast.origName) > 0){
                ifacesProvided[it.first] = nodeMap[nodeCast.origName];
            } else {
                std::cerr << "ISSUES 1 " << nodeCast.origName << std::endl;
            }
        } catch (std::bad_cast e){
            try{
                Topology topCast = dynamic_cast<Topology&>(*it.second.lock());
                if (topMap.count(topCast.origName) > 0){
                    ifacesProvided[it.first] = topMap[topCast.origName];
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

Topology::Topology(const Topology& temp): Positionable(temp), Nameable(temp) {
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
    ifacesAccepted = temp.ifacesAccepted;
    ifacesAcceptedSubNames = temp.ifacesAcceptedSubNames;
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