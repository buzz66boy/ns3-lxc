#include <string>
#include <iostream>
#include <memory>

#include "yaml-cpp/yaml.h"

#include "errorCode.h"
#include "node.h"
#include "application.h"
#include "topology.h"
#include "parserTags.h"
#include "topologyParser.h"
#include "commandParser.h"

using namespace std;

static void addCommandToTopology(std::string cmd, bool inherit, ns3lxc::Topology *top){
    for(auto nodePtr : top->nodes){
        nodePtr->commands.push_back(pair<string, bool>(cmd, inherit));
    }
    for(auto topPtr : top->subTopologies){
        addCommandToTopology(cmd, inherit, topPtr.get());
    }
}

static void parseCommandMap(YAML::Node cmds, ParsedTopology *parsedTop, bool inherit){
    for(auto cmdPair : cmds){
        string key = cmdPair.first.as<string>();
        if(key == TAG_INHERIT || key == pluralize(TAG_INHERIT)){
            continue;
        }
        vector<string> findMe = splitString(key);
        shared_ptr<ns3lxc::Node> nodePtr = findNode(findMe, &parsedTop->topology);
        switch(cmdPair.second.Type()){
            default:
            case(YAML::NodeType::Scalar):
                nodePtr->addCommand(cmdPair.second.as<string>(), inherit);
                // cout << "adding cmd: " + cmdPair.second.as<string>() + " to node " +nodePtr->name << endl;
                break;
            case(YAML::NodeType::Map):
                for(auto mapPair : cmdPair.second){
                    string appName = mapPair.first.as<string>();
                    bool appFound = false;
                    for(size_t i = 0; i < nodePtr->applications.size(); ++i){
                        ns3lxc::Application *app = &nodePtr->applications[i];
                        if(app->name == appName){
                            app->addCommand(mapPair.second.as<string>(), inherit);
                            // cout << "adding cmd: " + mapPair.second.as<string>() + " to app " + appName + " on node " +nodePtr->name << endl;
                            appFound = true;
                            break;
                        }
                    }
                    if(!appFound){
                        throw Ns3lxcException(ErrorCode::APP_NOT_FOUND, appName + " on " + nodePtr->name);
                    }
                }
                break;
            case(YAML::NodeType::Sequence):
                for(auto cmd : cmdPair.second){
                    // cout << "adding cmd: " + cmd.as<string>() + " to node " +nodePtr->name << endl;
                    nodePtr->addCommand(cmd.as<string>(), inherit);
                }
                break;
        }
    }
}

void parseCommands(YAML::Node cmds, ParsedTopology *parsedTop){
    if(cmds.Type() == YAML::NodeType::Scalar){
        // add command to all nodes
        addCommandToTopology(cmds.as<string>(), true, &parsedTop->topology);
    } else if (cmds.Type() == YAML::NodeType::Map){
        bool inherit = true;
        YAML::Node inheritNode;
        if(cmds[TAG_INHERIT]){
            inheritNode = cmds[TAG_INHERIT];
        } else if (cmds[pluralize(TAG_INHERIT)]){
            inheritNode = cmds[pluralize(TAG_INHERIT)];
        }
        if(inheritNode.Type() == YAML::NodeType::Scalar){
            inherit = inheritNode.as<bool>();
        } else if (inheritNode.Type() == YAML::NodeType::Map){
            parseCommandMap(inheritNode, parsedTop, inherit);
            //if more commands, don't inherit
            inherit = false;
        }
        parseCommandMap(cmds, parsedTop, inherit);
    } else if (cmds.Type() == YAML::NodeType::Sequence) {
        for(auto command : cmds){
            addCommandToTopology(command.as<string>(), true, &parsedTop->topology);
        }
    }
}