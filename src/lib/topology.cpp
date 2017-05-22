#include <string>
#include <memory>
#include <iostream>

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
    if(top->linkMap.count(linkName) > 0){
        return top->linkMap.at(linkName).get();
    } else {
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
}

static ns3lxc::Node *findNode(Topology *top, std::string nodeName){
    if(top->nodeMap.count(nodeName) > 0){
        return top->nodeMap.at(nodeName).get();
    } else {
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
}

Topology::Topology(Topology *temp): Nameable(*temp), Positionable(*temp) {
    size_t i;
    for(i = 0; i < temp->subTopologies.size(); ++i){
        std::shared_ptr<Topology> ptr = std::make_shared<Topology>(temp->subTopologies[i].get());
        subTopologies.push_back(ptr);
        topMap[ptr->origName] = ptr;
    }
    for(i = 0; i < temp->nodes.size(); ++i){
        std::shared_ptr<Node> ptr = std::make_shared<Node>(*temp->nodes[i], temp->nodes[i]->origName);
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
            // if(ifacePair.second->link != nullptr){
            //     std::string origName = ifacePair.second->link->origName;
            //     if(linkMap.count(origName) > 0){
            //         ifacePair.second->link = linkMap.at(origName).get();
            //         Link *l = linkMap.at(origName).get();
            //         bool eraseOccured = false;
            //         for(int j = 0; j < l->ifaces.size(); ++j){
            //             if(l->ifaces[j]->name == ifacePair.second->name){
            //                 l->ifaces.erase(l->ifaces.begin() + j);
            //                 eraseOccured = true;
            //                 break;
            //             }
            //         }
            //         if(!eraseOccured){
            //             std::cerr << "No Erase Occured for link " + l->name<< std::endl;
            //         }

            //         ifacePair.second->link->connectIface(ifacePair.second);
            //         Link::reRefIfaces(ifacePair.second->link);
            //     }
            // }
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
        //FIXME!!! store names read in originally in link (map to iface?)
        for(int n = 0; n < ptr->ifaces.size(); n++){
            ns3lxc::Node *no = ptr->ifaces[n]->node;
            std::cout << "N " << ptr->ifaces[n]->name << std::endl;
            std::string nodeName = ptr->ifaces[n]->node->name;
            ns3lxc::Node *nodePtr = findNode(this, nodeName);
            if(!nodePtr){
                std::cerr << "Node not found " + nodeName << std::endl;
            }
            //replace current iface with the correct one (from copied top)
            for(auto ifacePair : nodePtr->ifaces){
                if(ifacePair.second.name == ptr->ifaces[n]->name && ifacePair.second.link->name == ptr->name){
                    ptr->ifaces[n] = &ifacePair.second;
                    ifacePair.second.link = ptr.get();
                    std::cout << "NOOO " + (&ifacePair.second)->ip->str() << std::endl;
                    break;
                }
            }
        }
        ptr->reRefIfaces(ptr.get());
        links.push_back(ptr);
        linkMap[ptr->origName] = ptr;
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