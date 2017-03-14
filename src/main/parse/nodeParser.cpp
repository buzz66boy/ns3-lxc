#include <iostream>
#include <vector>
#include <string>

#include "yaml-cpp/yaml.h"

#include "node.h"
#include "topologyParser.h"
#include "nodeParser.h"

using namespace std;

std::vector<ns3lxc::Node> parseNode(YAML::Node node, ParsedTopology top){
    size_t iters = 1;
    vector<ns3lxc::Node> nodeList;

    if(node[TAG_NUM]){
        iters = node[TAG_NUM].as<int>();
    }

    for(size_t i = 0; i < iters; ++i){
        YAML::const_iterator it=node.begin();

        std::string name = node.begin()->first.as<std::string>();
        ns3lxc::Node parsedNode;

        if(iters > 1){
            name += "_" + std::to_string(i + 1); //start indexing at 1
        }
        if(node[TAG_TEMPLATE]){
            parsedNode = ns3lxc::Node( *top.nodes[node[TAG_TEMPLATE].as<std::string>()], node.as<std::string>());
        }
        nodeList.push_back(parsedNode);
    }
    return nodeList;
}