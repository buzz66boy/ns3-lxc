#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <sys/socket.h>

#include "yaml-cpp/yaml.h"

#include "ipaddr.h"
#include "linkTypeMap.h"
#include "parserTags.h"
#include "topologyParser.h"
#include "link.h"
#include "topology.h"
#include "errorCode.h"
#include "ifaceParser.h"
#include "linkParser.h"

using namespace std;

bool doesLinkExist(YAML::Node node, ParsedTopology *top){
    return (top->topology.linkMap.count(node.begin()->first.as<std::string>()) > 0);
}

static void parseLinkIfacesAccepted(YAML::Node node, ParsedTopology *top, shared_ptr<yntdl::Link> linkPtr){
        for(size_t i = 0; i < node.size(); ++i){
            string accName = node[i].as<string>();
            top->topology.ifacesAcceptedSubNames[accName] = "";
            top->topology.ifacesAccepted[accName] = linkPtr;
        }
}

void parseLink(YAML::Node node, ParsedTopology *top){
    vector<string> recognizedTags;
    std::string name = node.begin()->first.as<std::string>();
    node = node[name];

    std::cout << "parsing link " << name << endl;

    std::shared_ptr<yntdl::Link> link;
    if(node[TAG_TEMPLATE]){
        recognizedTags.push_back(TAG_TEMPLATE);
        link = std::make_shared<yntdl::Link>(name, *top->links[node[TAG_TEMPLATE].as<string>()]);
    } else {
        link = std::make_shared<yntdl::Link>(name);
    }

    if(node[TAG_TYPE]){
        recognizedTags.push_back(TAG_TYPE);
        link->setType(node[TAG_TYPE].as<std::string>());
        if(linkTypeMap.count(link->getType()) < 1){
            throw Ns3lxcException(ErrorCode::LINK_TYPE_NOT_FOUND, name);
        }
    } else {
        throw Ns3lxcException(ErrorCode::LINK_TYPE_NOT_SPECIFIED, name);
    }
    yntdl::IpAddr subnetMask(AF_INET, "255.255.255.0");
    if(node[TAG_CIDR]){
        recognizedTags.push_back(TAG_CIDR);
        int cidr = node[TAG_CIDR].as<int>();
        subnetMask = yntdl::IpAddr(AF_INET, cidr);
    } else if(node[TAG_SUBNET_MASK]){
        recognizedTags.push_back(TAG_SUBNET_MASK);
        subnetMask = yntdl::IpAddr(AF_INET, node[TAG_SUBNET_MASK].as<string>());
    }
    link->subnetMask = new yntdl::IpAddr(subnetMask);

    if(node[TAG_IFACES_ACCEPTED]){
        recognizedTags.push_back(TAG_IFACES_ACCEPTED);
        parseLinkIfacesAccepted(node[TAG_IFACES_ACCEPTED], top, link);
    }

    if(node[TAG_BANDWIDTH]){
        recognizedTags.push_back(TAG_BANDWIDTH);
        link->bandwidth = node[TAG_BANDWIDTH].as<string>();
    }

    if(node[TAG_LATENCY]){
        recognizedTags.push_back(TAG_LATENCY);
        link->latency = node[TAG_LATENCY].as<string>();
    }

    int numIfaces = 0;
    YAML::Node ifaceNode;

    if(node[TAG_IFACE]){
        recognizedTags.push_back(TAG_IFACE);
        ifaceNode = node[TAG_IFACE];
    } else if(node[pluralize(TAG_IFACE)]){
        recognizedTags.push_back(pluralize(TAG_IFACE));
        ifaceNode = node[pluralize(TAG_IFACE)];
    }  else if (node[TAG_INTERFACE]){
        recognizedTags.push_back(TAG_INTERFACE);
        ifaceNode = node[TAG_INTERFACE];
    } else if (node[pluralize(TAG_INTERFACE)]){
        recognizedTags.push_back(pluralize(TAG_INTERFACE));
        ifaceNode = node[pluralize(TAG_INTERFACE)];
    } 
    if(ifaceNode.Type() == YAML::NodeType::Sequence) {
        for(size_t i = 0; i < ifaceNode.size(); ++i){
            std::vector<std::string> split = splitString(ifaceNode[i].as<std::string>());
            cout << "\tconnecting " << split[0] << " " << split[1] << " with ip " << (split.size() > 2 ? split[2] : "") << endl;
            shared_ptr<yntdl::IfaceProvider> provPtr = getProvider(split[0], &top->topology).lock();
            yntdl::Iface *ifacePtr = provPtr->getIface(split[1]);
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
                ifacePtr->ip = new yntdl::IpAddr(AF_INET, split[2]);
                ifacePtr->subnetMask = new yntdl::IpAddr(subnetMask);
            }
        }
    }
    link->mapAdditionalTags(recognizedTags, node);
    if(top->topology.linkMap.count(link->name) > 0){
        cout << "LINK EXISTS" << link->name << endl;
    } else {
        top->topology.links.push_back(link);
        top->topology.linkMap[link->name] = link;
    }
}

void overrideLink(YAML::Node link, ParsedTopology *top){

}