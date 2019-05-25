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
#include "nodeTypeMap.h"
#include "parserTags.h"
#include "errorCode.h"
#include "settingsParser.h"
#include "topologyParser.h"
#include "positionParser.h"
#include "nodeParser.h"

using namespace std;

static void parseNodeIfaces(YAML::Node ifaces, std::shared_ptr<yntdl::Node> node){
    cout << "\tifaces:" << endl;
    if(node->ifaces.size() > 0){
        node->ifaces.clear();
    }
    if(ifaces.size() > 0){
        for(size_t i = 0; i < ifaces.size(); ++i){
            vector<string> ifaceNameMac = splitString(ifaces[i].as<std::string>());
            std::string name = ifaceNameMac[0];
            cout << "\t\t- " << name << endl;
            node->ifaces[name] = yntdl::Iface(name, node.get());
            if(ifaceNameMac.size() > 1){
                node->ifaces[name].macAddr = ifaceNameMac[1]; 
            } else {
                node->ifaces[name].macAddr = "xx:xx:xx:xx:xx:xx";
            }
        }
    } else {
        vector<string> ifaceNameMac = splitString(ifaces.as<std::string>());
        std::string name = ifaceNameMac[0];
        cout << "\t\t- " << name << endl;
        node->ifaces[name] = yntdl::Iface(name, node.get());
        if(ifaceNameMac.size() > 1){
            node->ifaces[name].macAddr = ifaceNameMac[1];   
        } else {
            node->ifaces[name].macAddr = "xx:xx:xx:xx:xx:xx";
        }
    }
}

void parseNodeApplications(YAML::Node apps, std::shared_ptr<yntdl::Node> node){
    if(node->applications.size() > 0){
        node->applications.clear();
    }
    for(size_t i = 0; i < apps.size(); ++i){
        string appName = apps[i].begin()->first.as<string>();
        string cmd = "";
        if(apps[i].begin()->second.Type() == YAML::NodeType::Scalar){
            //add ${} parsing
            cmd = apps[i].begin()->second.as<string>();
            node->applications.push_back(yntdl::Application(appName, cmd));
        } else if(apps[i].begin()->second.Type() == YAML::NodeType::Map) {
            yntdl::Application app(appName);
            app.mapAdditionalTags(vector<string>(), apps[i].begin()->second);
            node->applications.push_back(app);
        }
    }
}

void parseNodeCommands(YAML::Node cmds, std::shared_ptr<yntdl::Node> nodePtr){
    if(nodePtr->commands.size() > 0){
        nodePtr->commands.clear();
    }
    if(cmds.Type() == YAML::NodeType::Scalar){
        nodePtr->addCommand(cmds.as<string>(), true);
    } else if (cmds.Type() == YAML::NodeType::Sequence){
        for(auto cmd : cmds){
            nodePtr->addCommand(cmd.as<string>(), true);
        }
    }
}

std::vector<std::shared_ptr<yntdl::Node> > parseNode(YAML::Node node, ParsedTopology *top){
    size_t iters = 1;
    vector<string> recognizedTags;
    std::string origName = node.begin()->first.as<std::string>();
    node = node[origName];

    if(node[TAG_NUM]){
        iters = node[TAG_NUM].as<int>();
        recognizedTags.push_back(TAG_NUM);
    }

    vector<shared_ptr<yntdl::Node> > nodeList;

    for(size_t i = 0; i < iters; ++i){
        std::string name = origName;
        std::shared_ptr<yntdl::Node> nodePtr = nullptr;

        if(iters > 1){
            name += "_" + std::to_string(i + 1); //start indexing at 1
        }
        cout << "Node: " << name << endl;
        if(node[TAG_TEMPLATE]){
            recognizedTags.push_back(TAG_TEMPLATE);
            nodePtr = make_shared<yntdl::Node>(*top->nodes[node[TAG_TEMPLATE].as<std::string>()], name, name);
            yntdl::Node::reRefIfaces(nodePtr.get());
        } else {
            nodePtr = make_shared<yntdl::Node>(name);
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
        if(node[TAG_COMMAND]){
            parseNodeCommands(node[TAG_COMMAND], nodePtr);
        } else if(node[pluralize(TAG_COMMAND)]){
            parseNodeCommands(node[pluralize(TAG_COMMAND)], nodePtr);
        }

        if(node[TAG_TYPE]){
            recognizedTags.push_back(TAG_TYPE);
            nodePtr->type = node[TAG_TYPE].as<string>();
        } else {
            nodePtr->type = Settings::node_type;
        }
        if(nodeTypeMap.count(nodePtr->type) < 1){
            throw Ns3lxcException(ErrorCode::NODE_TYPE_NOT_FOUND, origName + " " + nodePtr->type);
        }

        if(node[TAG_POSITION]){
            recognizedTags.push_back(TAG_POSITION);
            if(iters > 1 && node[TAG_POSITION][name]){
                parsePositions(node[TAG_POSITION][name], nodePtr);
            } else if(node[TAG_POSITION].Type() == YAML::NodeType::Scalar) {
                parsePositions(node[TAG_POSITION], nodePtr);
            }
        } else if (node[pluralize(TAG_POSITION)]){
            recognizedTags.push_back(pluralize(TAG_POSITION));
            if(iters > 1 && node[pluralize(TAG_POSITION)][name]){
                YAML::Node baseNode = node[pluralize(TAG_POSITION)][name];
                parsePositions(baseNode[name], nodePtr);
            } else if (node[pluralize(TAG_POSITION)].Type() == YAML::NodeType::Scalar){
                parsePositions(node[pluralize(TAG_POSITION)], nodePtr);
            }
        }
        nodePtr->mapAdditionalTags(recognizedTags, node);
        nodeList.push_back(nodePtr);
    }
    return nodeList;
}