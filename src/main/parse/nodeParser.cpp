#include <iostream>
#include <vector>
#include <string>

#include "yaml-cpp/yaml.h"

#include "node.h"
#include "topologyParser.h"
#include "nodeParser.h"

using namespace std;

static std::vector<ns3lxc::Iface> parseNodeIfaces(YAML::Node ifaces, ns3lxc::Node node);

std::vector<ns3lxc::Node> parseNode(YAML::Node node, ParsedTopology top){
    size_t iters = 1;
    vector<ns3lxc::Node> nodeList;

    if(node[TAG_NUM]){
        iters = node[TAG_NUM].as<int>();
    }

    for(size_t i = 0; i < iters; ++i){
        std::string name = node.begin()->first.as<std::string>();
        ns3lxc::Node parsedNode;
        vector<ns3lxc::Iface> ifaceList;

        if(iters > 1){
            name += "_" + std::to_string(i + 1); //start indexing at 1
        }
        if(node[TAG_TEMPLATE]){
            parsedNode = ns3lxc::Node( *top.nodes[node[TAG_TEMPLATE].as<std::string>()], name);
        }
        if(node[TAG_IFACE]){
            ifaceList = parseNodeIfaces(node[TAG_IFACE], parsedNode);
        } else if (node[pluralize(TAG_IFACE)]){
            ifaceList = parseNodeIfaces(node[pluralize(TAG_IFACE)], parsedNode);
        }
        nodeList.push_back(parsedNode);
    }
    return nodeList;
}

static std::vector<ns3lxc::Iface> parseNodeIfaces(YAML::Node ifaces, ns3lxc::Node node){
    vector<ns3lxc::Iface> ifaceList;
    for(size_t i = 0; i < ifaces.size(); ++i){
        ns3lxc::Iface parsedIface;
        parsedIface.node = &node;
        parsedIface.name = ifaces[i].as<std::string>();
        ifaceList.push_back(parsedIface);
    }
    return ifaceList;
}