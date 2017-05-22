#include <string>
#include <iostream>
#include <map>

#include "yaml-cpp/yaml.h"

#include "ipaddr.h"
#include "parserTags.h"
#include "topologyParser.h"
#include "link.h"
#include "topology.h"
#include "linkTypeMap.h"
#include "linkParser.h"

using namespace std;

bool doesLinkExist(YAML::Node node, ParsedTopology *top){
    return (top->topology.linkMap.count(node.begin()->first.as<std::string>()) > 0);
}

std::shared_ptr<ns3lxc::Link> parseLink(YAML::Node node, ParsedTopology *top){
    
    std::string name = node.begin()->first.as<std::string>();
    node = node[name];

    std::cout << "parsing link " << name << endl;

    std::shared_ptr<ns3lxc::Link> link;
    if(node[TAG_TEMPLATE]){
        link = std::shared_ptr<ns3lxc::Link>(new ns3lxc::Link(name, *top->links[node[TAG_TEMPLATE].as<string>()]));
    } else {
        link = std::shared_ptr<ns3lxc::Link>(new ns3lxc::Link(name));
    }

    if(node[TAG_TYPE]){
        link->setType(node[TAG_TYPE].as<std::string>());
    } else {
        //err
        cerr << "No Type specified for link " << name << endl;
    }
    ns3lxc::IpAddr subnetIp(AF_INET, "255.255.255.0");
    if(node[TAG_CIDR]){
        int cidr = node[TAG_CIDR].as<int>();
        subnetIp = ns3lxc::IpAddr(AF_INET, cidr);
    } else if(node[TAG_SUBNET]){
        subnetIp = ns3lxc::IpAddr(AF_INET, node[TAG_SUBNET].as<string>());
    }
    int numIfaces = 0;
    YAML::Node ifaceNode;

    if(node[TAG_IFACE]){
        ifaceNode = node[TAG_IFACE];
    } else if(node[pluralize(TAG_IFACE)]){
        ifaceNode = node[pluralize(TAG_IFACE)];
    }
    if(ifaceNode.Type() == YAML::NodeType::Scalar){
        link->setNumIface(ifaceNode.as<int>());
    } else {
        for(size_t i = 0; i < ifaceNode.size(); ++i){
            std::vector<std::string> split = splitString(ifaceNode[i].as<std::string>());
            cout << "\tconnecting " << split[0] << " " << split[1] << " with ip " << (split.size() > 2 ? split[2] : "") << endl;
            if(top->topology.nodeMap.count(split[0]) > 0){
                shared_ptr<ns3lxc::Node> nodePtr = top->topology.nodeMap[split[0]];
                if(!nodePtr){
                    cout << "NO NODE" << endl;
                    //err
                }
                ns3lxc::Iface *ifacePtr = nodePtr->getIface(split[1]);
                if(!ifacePtr){
                    cout << "NO IFACE" << endl;
                    //err
                }
                link->connectIface(ifacePtr);
                if(split.size() > 2){
                    //handle ip
                    if(ifacePtr->ip == nullptr){
                        delete ifacePtr->ip;
                    }
                    if(ifacePtr->subnetMask == nullptr){
                        delete ifacePtr->ip;
                    }
                    ifacePtr->ip = new ns3lxc::IpAddr(AF_INET, split[2]);
                    ifacePtr->subnetMask = new ns3lxc::IpAddr(AF_INET, "255.255.255.0");
                }
            } else if(top->topology.topMap.count(split[0]) > 0){
                shared_ptr<ns3lxc::Topology> topPtr = top->topology.topMap[split[0]];
                if(!topPtr){
                    cout << "NO TOP" << endl;
                    //err
                }
                ns3lxc::Iface *ifacePtr = topPtr->getIface(split[1]);
                if(!ifacePtr){
                    cout << "NO IFACE " << split[1] << " "<< topPtr->ifacesProvidedSubNames.size() << endl;
                    //err
                    for(auto it: topPtr->ifacesProvidedSubNames){
                        cout << it.first << endl;
                    }
                }
                link->connectIface(ifacePtr);
                if(split.size() > 2){
                    //handle ip
                    if(ifacePtr->ip == nullptr){
                        delete ifacePtr->ip;
                    }
                    if(ifacePtr->subnetMask == nullptr){
                        delete ifacePtr->subnetMask;
                    }
                    ifacePtr->ip = new ns3lxc::IpAddr(AF_INET, split[2]);
                    ifacePtr->subnetMask = new ns3lxc::IpAddr(AF_INET, "255.255.255.0");
                }
            } else {
                // Error
                cerr << split[0] << " not found" << endl;
                exit(13);
            }
        }
    }
    return link;
}

void overrideLink(YAML::Node link, ParsedTopology *top){

}