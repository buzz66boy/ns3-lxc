#include <string>
#include <iostream>
#include <map>

#include "yaml-cpp/yaml.h"

#include "parserTags.h"
#include "topologyParser.h"
#include "link.h"
#include "topology.h"
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
            cout << "\tconnecting " << split[0] << " " << split[1] << endl;
            if(top->topology.nodeMap.count(split[0]) > 0){
                shared_ptr<ns3lxc::Node> nodePtr = top->topology.nodeMap[split[0]];
                if(!nodePtr){
                    cout << "NO NODE" << endl;
                    //err
                }
                shared_ptr<ns3lxc::Iface> ifacePtr = nodePtr->getIface(split[1]).lock();
                if(!ifacePtr){
                    cout << "NO IFACE" << endl;
                    //err
                }
                link->connectIface(ifacePtr);
            } else if(top->topology.topMap.count(split[0]) > 0){
                shared_ptr<ns3lxc::Topology> topPtr = top->topology.topMap[split[0]];
                if(!topPtr){
                    cout << "NO TOP" << endl;
                    //err
                }
                shared_ptr<ns3lxc::Iface> ifacePtr = topPtr->getIface(split[1]).lock();
                if(!ifacePtr){
                    cout << "NO IFACE" << endl;
                    //err
                }
                link->connectIface(ifacePtr);
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