#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>

#include <arpa/inet.h>

#include "yaml-cpp/yaml.h"

#include "nodeTypeMap.h"
#include "node.h"
#include "iface.h"
#include "parserTags.h"
#include "errorCode.h"
#include "settingsParser.h"
#include "topologyParser.h"
#include "positionParser.h"
#include "nodeParser.h"

using namespace std;

static void parseNodeIfaces(YAML::Node ifaces, std::shared_ptr<ns3lxc::Node> node){
    cout << "\tifaces:" << endl;
    if(ifaces.size() > 0){
        for(size_t i = 0; i < ifaces.size(); ++i){
            vector<string> ifaceNameMac = splitString(ifaces[i].as<std::string>());
            std::string name = ifaceNameMac[0];
            cout << "\t\t- " << name << endl;
            node->ifaces[name] = ns3lxc::Iface(name, node.get());
            if(ifaceNameMac.size() > 1){
                node->ifaces[name].macAddr = ifaceNameMac[1]; 
            }
        }
    } else {
        vector<string> ifaceNameMac = splitString(ifaces.as<std::string>());
        std::string name = ifaceNameMac[0];
        cout << "\t\t- " << name << endl;
        node->ifaces[name] = ns3lxc::Iface(name, node.get());
        if(ifaceNameMac.size() > 1){
            node->ifaces[name].macAddr = ifaceNameMac[1];   
        } else {
            node->ifaces[name].macAddr = "xx:xx:xx:xx:xx:xx";
        }
    }
}

void parseNodeApplications(YAML::Node apps, std::shared_ptr<ns3lxc::Node> node){
    for(size_t i = 0; i < apps.size(); ++i){
        string appName = apps[i].begin()->first.as<string>();
        string args = "";
        if(apps[i].begin()->second.Type() == YAML::NodeType::Scalar){
            //add ${} parsing
            args = apps[i].begin()->second.as<string>();
            node->applications.push_back(ns3lxc::Application(appName, args));
        } else if(apps[i].begin()->second.Type() == YAML::NodeType::Map) {
            ns3lxc::Application app(appName);
            for(auto pair : apps[i].begin()->second){
                app.additionalTags[pair.first.as<string>()] = pair.second;
            }
            node->applications.push_back(app);
        }
    }
}

std::vector<std::shared_ptr<ns3lxc::Node> > parseNode(YAML::Node node, ParsedTopology *top){
    size_t iters = 1;
    vector<string> recognizedTags;
    std::string origName = node.begin()->first.as<std::string>();
    node = node[origName];

    if(node[TAG_NUM]){
        iters = node[TAG_NUM].as<int>();
        recognizedTags.push_back(TAG_NUM);
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
            recognizedTags.push_back(TAG_TEMPLATE);
            nodePtr = make_shared<ns3lxc::Node>(*top->nodes[node[TAG_TEMPLATE].as<std::string>()], name, name);
            ns3lxc::Node::reRefIfaces(nodePtr.get());
        } else {
            nodePtr = make_shared<ns3lxc::Node>(name);
        }
        if(node[TAG_IFACE]){
            recognizedTags.push_back(TAG_IFACE);
            parseNodeIfaces(node[TAG_IFACE], nodePtr);
        } else if (node[pluralize(TAG_IFACE)]){
            recognizedTags.push_back(pluralize(TAG_IFACE));
            parseNodeIfaces(node[pluralize(TAG_IFACE)], nodePtr);
        } else if (node[TAG_INTERFACE]){
            recognizedTags.push_back(TAG_INTERFACE);
            parseNodeIfaces(node[TAG_INTERFACE], nodePtr);
        } else if (node[pluralize(TAG_INTERFACE)]){
            recognizedTags.push_back(pluralize(TAG_INTERFACE));
            parseNodeIfaces(node[pluralize(TAG_INTERFACE)], nodePtr);
        }
        if(node[TAG_APPLICATION]){
            recognizedTags.push_back(TAG_APPLICATION);
            parseNodeApplications(node[TAG_APPLICATION], nodePtr);
        } else if (node[pluralize(TAG_APPLICATION)]){
            recognizedTags.push_back(pluralize(TAG_APPLICATION));
            parseNodeApplications(node[pluralize(TAG_APPLICATION)], nodePtr);
        }

        if(node[TAG_TYPE]){
            recognizedTags.push_back(TAG_TYPE);
            nodePtr->type = node[TAG_TYPE].as<string>();
            if(nodeTypeMap.count(nodePtr->type) < 0){
                throw Ns3lxcException(ErrorCode::NODE_TYPE_NOT_FOUND, origName + " " + nodePtr->type);
            }
        } else {
            nodePtr->type = Settings::container_type;
        }

        if(node[TAG_POSITION]){
            recognizedTags.push_back(TAG_POSITION);
            if(iters > 1){
                if(node[TAG_POSITION][name]){
                    parsePositions(node[TAG_POSITION][name], nodePtr);
                }
            } else {
                parsePositions(node[TAG_POSITION], nodePtr);
            }
        } else if (node[pluralize(TAG_POSITION)]){
            recognizedTags.push_back(pluralize(TAG_POSITION));
            if(iters > 1){
                YAML::Node baseNode = node[pluralize(TAG_POSITION)];
                if(baseNode[name]){
                    parsePositions(baseNode[name], nodePtr);
                }
            } else {
                parsePositions(node[pluralize(TAG_POSITION)], nodePtr);
            }
        }
        nodePtr->mapAdditionalTags(recognizedTags, node);
        nodeList.push_back(nodePtr);
    }
    return nodeList;
}