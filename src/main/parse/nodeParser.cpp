#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>

#include <arpa/inet.h>

#include "yaml-cpp/yaml.h"

#include "node.h"
#include "iface.h"
#include "parserTags.h"
#include "topologyParser.h"
#include "nodeParser.h"

using namespace std;

static void parseNodeIfaces(YAML::Node ifaces, std::shared_ptr<ns3lxc::Node> node){
    cout << "\tifaces:" << endl;
    for(size_t i = 0; i < ifaces.size(); ++i){
        std::string name = ifaces[i].as<std::string>();
        cout << "\t\t- " << name << endl;
        node->ifaces[name] = std::shared_ptr<ns3lxc::Iface>(new ns3lxc::Iface(name, &(*node)));
        node->ifaces[name]->bridgeName = node->name + "_" + to_string(i) + "_b";
        node->ifaces[name]->tapName = node->name + "_" + to_string(i) + "_t";
    }
}

std::vector<std::shared_ptr<ns3lxc::Node> > parseNode(YAML::Node node, ParsedTopology *top){
    size_t iters = 1;
    std::string origName = node.begin()->first.as<std::string>();
    node = node[origName];

    if(node[TAG_NUM]){
        iters = node[TAG_NUM].as<int>();
    }

    vector<shared_ptr<ns3lxc::Node> > nodeList;

    for(size_t i = 0; i < iters; ++i){
        std::string name = origName;
        std::shared_ptr<ns3lxc::Node> nodePtr = nullptr;

        if(iters > 1){
            name += "_" + std::to_string(i + 1); //start indexing at 1
        }
        cout << "Node: " << name << endl;
        if(node[TAG_TEMPLATE]){
            nodePtr = shared_ptr<ns3lxc::Node>(new ns3lxc::Node( *top->nodes[node[TAG_TEMPLATE].as<std::string>()], name));
        } else {
            nodePtr = shared_ptr<ns3lxc::Node>(new ns3lxc::Node(name));
        }
        if(node[TAG_IFACE]){
            parseNodeIfaces(node[TAG_IFACE], nodePtr);
        } else if (node[pluralize(TAG_IFACE)]){
            parseNodeIfaces(node[pluralize(TAG_IFACE)], nodePtr);
        }
        nodeList.push_back(nodePtr);
    }
    return nodeList;
}