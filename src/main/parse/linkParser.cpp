#include <string>

#include "yaml-cpp/yaml.h"

#include "parserTags.h"
#include "topologyParser.h"
#include "link.h"
#include "linkParser.h"

using namespace std;

ns3lxc::Link parseLink(YAML::Node node, ParsedTopology top){
    YAML::Node ifaceNode;

    std::string name = node.begin()->first.as<std::string>();
    node = node[name];

    ns3lxc::Link link = ns3lxc::Link(name);
    if(node[TAG_TEMPLATE]){
        ns3lxc::Link link = ns3lxc::Link(name, *top.links[node[TAG_TEMPLATE].as<string>()]);
    }
    int numIfaces = 0;

    if(node[TAG_IFACE]){
        ifaceNode = node[TAG_IFACE];
    } else if(node[pluralize(TAG_IFACE)]){
        ifaceNode = node[pluralize(TAG_IFACE)];
    }
    if(ifaceNode.Type() == YAML::NodeType::Scalar){
        link.setNumIface(ifaceNode.as<int>());
    } else {
        for(size_t i = 0; i < ifaceNode.size(); ++i){
            std::vector<std::string> split = splitString(ifaceNode[i].as<std::string>());
            if(top.topology.nodeMap[split[0]]){
                link.connectIface(top.topology.nodeMap[split[0]]->getIface(split[1]));
            } else {
                // Error
            }
        }
    }
    return link;
}