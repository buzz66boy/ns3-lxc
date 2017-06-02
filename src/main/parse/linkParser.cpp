#include <string>
#include <iostream>
#include <map>
#include <sys/socket.h>

#include "yaml-cpp/yaml.h"

#include "ipaddr.h"
#include "parserTags.h"
#include "topologyParser.h"
#include "link.h"
#include "topology.h"
#include "linkTypeMap.h"
#include "errorCode.h"
#include "ifaceParser.h"
#include "linkParser.h"

using namespace std;

bool doesLinkExist(YAML::Node node, ParsedTopology *top){
    return (top->topology.linkMap.count(node.begin()->first.as<std::string>()) > 0);
}

static void parseLinkIfacesAccepted(YAML::Node node, ParsedTopology *top, shared_ptr<ns3lxc::Link> linkPtr){
        for(size_t i = 0; i < node.size(); ++i){
            string accName = node[i].as<string>();
            top->topology.ifacesAcceptedSubNames[accName] = "";
            top->topology.ifacesAccepted[accName] = linkPtr;
        }
}

void parseLink(YAML::Node node, ParsedTopology *top){
    std::string name = node.begin()->first.as<std::string>();
    node = node[name];

    std::cout << "parsing link " << name << endl;

    std::shared_ptr<ns3lxc::Link> link;
    if(node[TAG_TEMPLATE]){
        link = std::make_shared<ns3lxc::Link>(name, *top->links[node[TAG_TEMPLATE].as<string>()]);
    } else {
        link = std::make_shared<ns3lxc::Link>(name);
    }

    if(node[TAG_TYPE]){
        link->setType(node[TAG_TYPE].as<std::string>());
    } else {
        throw Ns3lxcException(ErrorCode::LINK_TYPE_NOT_SPECIFIED, name);
    }
    ns3lxc::IpAddr subnetIp(AF_INET, "255.255.255.0");
    if(node[TAG_CIDR]){
        int cidr = node[TAG_CIDR].as<int>();
        subnetIp = ns3lxc::IpAddr(AF_INET, cidr);
    } else if(node[TAG_SUBNET]){
        subnetIp = ns3lxc::IpAddr(AF_INET, node[TAG_SUBNET].as<string>());
    }

    if(node[TAG_IFACES_ACCEPTED]){
        parseLinkIfacesAccepted(node[TAG_IFACES_ACCEPTED], top, link);
    }

    if(node[TAG_BANDWIDTH]){
        link->bandwidth = node[TAG_BANDWIDTH].as<string>();
    }

    if(node[TAG_LATENCY]){
        link->latency = node[TAG_LATENCY].as<string>();
    }

    int numIfaces = 0;
    YAML::Node ifaceNode;

    if(node[TAG_IFACE]){
        ifaceNode = node[TAG_IFACE];
    } else if(node[pluralize(TAG_IFACE)]){
        ifaceNode = node[pluralize(TAG_IFACE)];
    }  else if (node[TAG_INTERFACE]){
        ifaceNode = node[TAG_INTERFACE];
    } else if (node[pluralize(TAG_INTERFACE)]){
        ifaceNode = node[pluralize(TAG_INTERFACE)];
    }
    if(ifaceNode.Type() == YAML::NodeType::Scalar){
        link->setNumIface(ifaceNode.as<int>());
    } else if(ifaceNode.Type() != YAML::NodeType::Null) {
        for(size_t i = 0; i < ifaceNode.size(); ++i){
            std::vector<std::string> split = splitString(ifaceNode[i].as<std::string>());
            cout << "\tconnecting " << split[0] << " " << split[1] << " with ip " << (split.size() > 2 ? split[2] : "") << endl;
            shared_ptr<ns3lxc::IfaceProvider> provPtr = getProvider(split[0], &top->topology).lock();
            ns3lxc::Iface *ifacePtr = provPtr->getIface(split[1]);
            if(!ifacePtr){
                throw Ns3lxcException(ErrorCode::IFACE_NOT_FOUND, split[1]);
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
                ifacePtr->subnetMask = new ns3lxc::IpAddr(subnetIp);
            }
        }
    }
    if(top->topology.linkMap.count(link->name) > 0){
        cout << "LINK EXISTS" << link->name << endl;
    } else {
        top->topology.links.push_back(link);
        top->topology.linkMap[link->name] = link;
    }
}

void overrideLink(YAML::Node link, ParsedTopology *top){

}