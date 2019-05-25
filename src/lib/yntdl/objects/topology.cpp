#include <string>
#include <memory>
#include <iostream>

#include "errorCode.h"
#include "ifaceParser.h"
#include "node.h"
#include "topology.h"

using namespace ns3lxc;

Topology::Topology(){
    
}

Topology::Topology(std::shared_ptr<Topology> temp, std::string newName): Topology(temp.get()){
    name = newName;
    origName = newName;
}

static Link *findLink(Topology *top, std::string linkName){
    for(auto linkPtr : top->links){
        if(linkPtr->name == linkName){
            return linkPtr.get();
        }
    }
    for(auto topPtr : top->subTopologies){
        Link *ln = findLink(topPtr.get(), linkName);
        if(ln != nullptr){
            return ln;
        }
    }
    return nullptr;
}

static ns3lxc::Node *findNode(Topology *top, std::string nodeName){
    for(auto nodePtr : top->nodes){
        if(nodePtr->name == nodeName){
            return nodePtr.get();
        }
    }
    for(auto topPtr : top->subTopologies){
        ns3lxc::Node *n = findNode(topPtr.get(), nodeName);
        if(n != nullptr){
            return n;
        }
    }
    return nullptr;
}

Topology::Topology(Topology *temp): Nameable(*temp), Positionable(*temp) {
    size_t i;
    // std::cout << "COPYING " << temp->name << std::endl;
    for(i = 0; i < temp->subTopologies.size(); ++i){
        std::shared_ptr<Topology> ptr = std::make_shared<Topology>(temp->subTopologies[i].get());
        subTopologies.push_back(ptr);
        topMap[ptr->origName] = ptr;
    }
    for(i = 0; i < temp->nodes.size(); ++i){
        std::shared_ptr<Node> ptr = std::make_shared<Node>(*temp->nodes[i], temp->nodes[i]->name, temp->nodes[i]->origName);
        nodes.push_back(ptr);
        nodeMap[ptr->origName] = ptr;
        Node::reRefIfaces(ptr.get());
        for(auto ifacePair : ptr->ifaces){
            if(ifacePair.second.ip != nullptr){
                ifacePair.second.ip = new ns3lxc::IpAddr(*ifacePair.second.ip);
            }
            if(ifacePair.second.subnetMask != nullptr){
                ifacePair.second.subnetMask = new ns3lxc::IpAddr(*ifacePair.second.subnetMask);
            }
        }
    }
    for(i = 0; i < temp->links.size(); ++i){
        std::shared_ptr<Link> ptr = std::make_shared<Link>(*temp->links[i]);
        if(ptr->ip != nullptr){
            ptr->ip = new ns3lxc::IpAddr(*ptr->ip);
        }
        if(ptr->subnetMask != nullptr){
            ptr->subnetMask = new ns3lxc::IpAddr(*ptr->subnetMask);
        }
        links.push_back(ptr);
        linkMap[ptr->origName] = ptr;

    }
    for(auto linkPtr : links){
        // std::cout << "processing link: " + linkPtr->name << std::endl;
        for(i = 0; i < linkPtr->ifaces.size(); ++i){
            // std::cout << std::to_string(i) << std::endl;
            Iface *ifacePtr = linkPtr->ifaces[i];
            Node *foundNode = findNode(this, ifacePtr->node->name);
            if(!foundNode){
                throw Ns3lxcException(ErrorCode::NODE_NOT_FOUND, ifacePtr->node->name);
            }
            if(foundNode->ifaces.count(ifacePtr->name)){
                linkPtr->ifaces[i] = &foundNode->ifaces.at(ifacePtr->name);
            } else {
                throw Ns3lxcException(ErrorCode::IFACE_NOT_FOUND, ifacePtr->name);
            }
        }
        Link::reRefIfaces(linkPtr.get());
    }
    for(i = 0; i < temp->applications.size(); ++i){
        std::shared_ptr<Application> ptr = std::make_shared<Application>(*temp->applications[i]);
        applications.push_back(ptr);
    }
    ifacesProvidedSubNames.insert(temp->ifacesProvidedSubNames.begin(), temp->ifacesProvidedSubNames.end());
    for(auto it: temp->ifacesProvided){
        try {
            Node nodeCast = dynamic_cast<Node&>(*it.second.lock());
            if(nodeMap.count(nodeCast.origName) > 0){
                ifacesProvided[it.first] = nodeMap[nodeCast.origName];
            } else {
                throw Ns3lxcException(ErrorCode::NODE_NOT_FOUND, nodeCast.origName);
            }
        } catch (std::bad_cast e){
            try{
                Topology topCast = dynamic_cast<Topology&>(*it.second.lock());
                if (topMap.count(topCast.origName) > 0){
                    ifacesProvided[it.first] = topMap[topCast.origName];
                } else {
                    throw Ns3lxcException(ErrorCode::TOPOLOGY_NOT_FOUND, topCast.origName);
                }
            } catch (std::bad_cast e){
                throw Ns3lxcException(ErrorCode::PROVIDER_NOT_FOUND, it.first);
            }
        }
    }
    ifacesAcceptedSubNames.insert(temp->ifacesAcceptedSubNames.begin(), temp->ifacesAcceptedSubNames.end());
    for(auto it: temp->ifacesAccepted){
        try {
            Link linkCast = dynamic_cast<Link&>(*it.second.lock());
            if(linkMap.count(linkCast.origName) > 0){
                ifacesAccepted[it.first] = linkMap[linkCast.origName];
            } else {
                throw Ns3lxcException(ErrorCode::LINK_NOT_FOUND, linkCast.origName);
            }
        } catch (std::bad_cast e){
            try{
                Topology topCast = dynamic_cast<Topology&>(*it.second.lock());
                if (topMap.count(topCast.origName) > 0){
                    ifacesAccepted[it.first] = topMap[topCast.origName];
                } else {
                    throw Ns3lxcException(ErrorCode::TOPOLOGY_NOT_FOUND, topCast.origName);
                }
            } catch (std::bad_cast e){
                throw Ns3lxcException(ErrorCode::ACCEPTOR_NOT_FOUND, it.first);
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
    if(temp.ip){
        ip = new IpAddr(*temp.ip);
    }
    if(temp.subnetMask){
        subnetMask = new IpAddr(*temp.subnetMask);
    }
}

Topology::~Topology(){
    if(ip){
        delete ip;
    }
    if(subnetMask){
        delete subnetMask;
    }

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