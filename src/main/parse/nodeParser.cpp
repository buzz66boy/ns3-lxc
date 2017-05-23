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
#include "positionParser.h"
#include "nodeParser.h"

using namespace std;

static void parseNodeIfaces(YAML::Node ifaces, std::shared_ptr<ns3lxc::Node> node){
    cout << "\tifaces:" << endl;
    if(ifaces.size() > 0){
        for(size_t i = 0; i < ifaces.size(); ++i){
            std::string name = ifaces[i].as<std::string>();
            cout << "\t\t- " << name << endl;
            node->ifaces[name] = ns3lxc::Iface(name, node.get());
            //FIXME
            node->ifaces[name].subnetMask = new ns3lxc::IpAddr(AF_INET, "255.255.255.0");
        }
    } else {
        std::string name = ifaces.as<std::string>();
        cout << "\t\t- " << name << endl;
        node->ifaces[name] = ns3lxc::Iface(name, node.get());
        node->ifaces[name].subnetMask = new ns3lxc::IpAddr(AF_INET, "255.255.255.0");
    }
}

void parseNodeApplications(YAML::Node apps, std::shared_ptr<ns3lxc::Node> node){
    for(size_t i = 0; i < apps.size(); ++i){
        string appName = apps[i].begin()->first.as<string>();
        string args = "";
        if(apps[i].begin()->second.Type() != YAML::NodeType::Null){
            //add ${} parsing
            args = apps[i].begin()->second.as<string>();
        }
        node->applications.push_back(ns3lxc::Application(appName, args));
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
            nodePtr = make_shared<ns3lxc::Node>(*top->nodes[node[TAG_TEMPLATE].as<std::string>()], name, name);
            ns3lxc::Node::reRefIfaces(nodePtr.get());
        } else {
            nodePtr = make_shared<ns3lxc::Node>(name);
        }
        if(node[TAG_IFACE]){
            parseNodeIfaces(node[TAG_IFACE], nodePtr);
        } else if (node[pluralize(TAG_IFACE)]){
            parseNodeIfaces(node[pluralize(TAG_IFACE)], nodePtr);
        }
        if(node[TAG_APPLICATION]){
            parseNodeApplications(node[TAG_APPLICATION], nodePtr);
        } else if (node[pluralize(TAG_APPLICATION)]){
            parseNodeApplications(node[pluralize(TAG_APPLICATION)], nodePtr);
        }
        if(node[TAG_POSITION]){
            if(iters > 1){
                if(node[TAG_POSITION][name]){
                    parsePositions(node[TAG_POSITION][name], nodePtr);
                }
            } else {
                parsePositions(node[TAG_POSITION], nodePtr);
            }
        } else if (node[pluralize(TAG_POSITION)]){
            if(iters > 1){
                YAML::Node baseNode = node[pluralize(TAG_POSITION)];
                if(baseNode[name]){
                    parsePositions(baseNode[name], nodePtr);
                }
            } else {
                parsePositions(node[pluralize(TAG_POSITION)], nodePtr);
            }
        }
        nodeList.push_back(nodePtr);
    }
    return nodeList;
}