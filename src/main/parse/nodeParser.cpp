#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include "yaml-cpp/yaml.h"

#include "node.h"
#include "topologyParser.h"
#include "nodeParser.h"

using namespace std;

static std::vector<std::shared_ptr<ns3lxc::Iface> > parseNodeIfaces(YAML::Node ifaces, ns3lxc::Node *node);

std::vector<ns3lxc::Node> parseNode(YAML::Node node, ParsedTopology top){
    size_t iters = 1;
    std::string origName = node.begin()->first.as<std::string>();
    node = node[origName];

    if(node[TAG_NUM]){
        iters = node[TAG_NUM].as<int>();
    }

    vector<ns3lxc::Node> nodeList(iters);

    for(size_t i = 0; i < iters; ++i){
        std::string name = origName;
        std::vector<std::shared_ptr<ns3lxc::Iface> > ifaceList;

        if(iters > 1){
            name += "_" + std::to_string(i + 1); //start indexing at 1
        }
        if(node[TAG_TEMPLATE]){
            nodeList[i] = ns3lxc::Node( *top.nodes[node[TAG_TEMPLATE].as<std::string>()], name);
        } else {
            nodeList[i] = ns3lxc::Node(name);
        }
        if(node[TAG_IFACE]){
            ifaceList = parseNodeIfaces(node[TAG_IFACE], &nodeList[i]);
        } else if (node[pluralize(TAG_IFACE)]){
            ifaceList = parseNodeIfaces(node[pluralize(TAG_IFACE)], &nodeList[i]);
        }
        cout << "node: " << name << endl;
        for(size_t j = 0; j < ifaceList.size(); j++){
            nodeList[i].ifaces[ifaceList[j]->name] = ifaceList[j];
        }
    }
    return nodeList;
}

static std::vector<std::shared_ptr<ns3lxc::Iface> > parseNodeIfaces(YAML::Node ifaces, ns3lxc::Node *node){
    vector<std::shared_ptr<ns3lxc::Iface> > ifaceList(ifaces.size());
    for(size_t i = 0; i < ifaces.size(); ++i){
        ifaceList[i] = std::shared_ptr<ns3lxc::Iface>(new ns3lxc::Iface);
        ifaceList[i]->node = node;
        ifaceList[i]->name = ifaces[i].as<std::string>();
    }
    return ifaceList;
}