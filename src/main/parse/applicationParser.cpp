#include <string>
#include <iostream>
#include <memory>
#include <algorithm>

#include "yaml-cpp/yaml.h"

#include "application.h"
#include "node.h"
#include "topology.h"
#include "errorCode.h"
#include "parserTags.h"
#include "applicationParser.h"

using namespace std;

static void addAppToAllNodes(ns3lxc::Application *app, ns3lxc::Topology *top){
    for(auto subTopPtr : top->subTopologies){
        addAppToAllNodes(app, subTopPtr.get());
    }
    for(auto nodePtr : top->nodes){
        nodePtr->applications.push_back(*app);
    }
}

static void addAppToNode(ns3lxc::Application *application, std::string nodeName, ParsedTopology *parsedTop){
    cout << "adding application: " + application->name + " to node " + nodeName << endl;
    for(auto cmdPair : application->commands){
        cout << "\tcmd: " + cmdPair.first << endl;
    }
    vector<string> findMe = splitString(nodeName);
    shared_ptr<ns3lxc::Node> nodePtr = findNode(findMe, &parsedTop->topology);
    bool hasApp = false;
    ns3lxc::Application *appPtr;
    for(size_t i = 0; i < nodePtr->applications.size(); ++i){
        ns3lxc::Application *app = &nodePtr->applications[i];
        if(app->name == application->name){
            hasApp = true;
            appPtr = app;
            break;
        }
    }
    if(!hasApp){
        nodePtr->applications.push_back(application);
    } else {
        // Merge pre-existing app with new one
        for(auto cmdPair : application->commands){
            appPtr->commands.push_back(cmdPair);
        }
        if(application->path != "" && application->path != appPtr->path){
            appPtr->path = application->path;
        }
    }
}

static void parseMappedApplication(YAML::Node mapNode, string appName, ParsedTopology *parsedTop){
    vector<string> recognizedTags;
    if(mapNode[TAG_ALL]){
        recognizedTags.push_back(TAG_ALL);
        ns3lxc::Application app(appName);
        if(mapNode[TAG_ALL].Type() != YAML::NodeType::Null){
            //default args are present
            app.addCommand(mapNode[TAG_ALL].as<string>());
        }
        addAppToAllNodes(&app, &parsedTop->topology);
    }
    if(!mapNode[TAG_NODE] && !mapNode[pluralize(TAG_NODE)] && !mapNode[TAG_ALL]){
        throw Ns3lxcException(ErrorCode::NODE_NOT_SPECIFIED, appName);
    }
    bool inherit = true;
    if(mapNode[TAG_INHERIT]){
        recognizedTags.push_back(TAG_INHERIT);
        inherit = mapNode[TAG_INHERIT].as<bool>();
    } else if(mapNode[pluralize(TAG_INHERIT)]){
        recognizedTags.push_back(pluralize(TAG_INHERIT));
        inherit = mapNode[pluralize(TAG_INHERIT)].as<bool>();
    }
    ns3lxc::Application app(appName, inherit);
    YAML::Node cmdTag;
    if(mapNode[TAG_COMMAND]){
        recognizedTags.push_back(TAG_COMMAND);
        cmdTag = mapNode[TAG_COMMAND];
    } else if(mapNode[pluralize(TAG_COMMAND)]){
        recognizedTags.push_back(pluralize(TAG_COMMAND));
        cmdTag = mapNode[pluralize(TAG_COMMAND)];
    }
    if(cmdTag.Type() == YAML::NodeType::Scalar){
        app.addCommand(cmdTag.as<string>(), inherit);
    } else if(cmdTag.Type() == YAML::NodeType::Sequence){
        for(auto cmd : cmdTag){
            app.addCommand(cmd.as<string>(), inherit);
        }
    }

    YAML::Node nodeTag;
    if(mapNode[TAG_NODE]){
        recognizedTags.push_back(TAG_NODE);
        nodeTag = mapNode[TAG_NODE];
    } else if (mapNode[pluralize(TAG_NODE)]) {
        recognizedTags.push_back(pluralize(TAG_NODE));
        nodeTag = mapNode[pluralize(TAG_NODE)];
    }
    if(nodeTag.Type() != YAML::NodeType::Null){
        for(auto node : nodeTag){
            if(node.Type() == YAML::NodeType::Scalar){
                //Just a node name
                addAppToNode(&app, node.as<string>(), parsedTop);
            } else if(node.Type() == YAML::NodeType::Map){
                //Map of node names to commands
                string nodeName = node.begin()->first.as<string>();
                string cmd = node.begin()->second.as<string>();
                ns3lxc::Application nodeApp(app);
                nodeApp.addCommand(cmd);
                addAppToNode(&nodeApp, nodeName, parsedTop);
            }
        }
    }
}

void parseApplications(YAML::Node apps, ParsedTopology *parsedTop){
    for(size_t i = 0; i < apps.size(); ++i){
        string appName = apps[i].begin()->first.as<string>();
        switch(apps[i].begin()->second.Type()){
            case YAML::NodeType::Scalar:
                if(apps[i].begin()->second.as<string>() == "all"){
                    ns3lxc::Application app(appName);
                    addAppToAllNodes(&app, &parsedTop->topology);
                } else {
                    ns3lxc::Application app(appName);
                    addAppToNode(&app, apps[i].begin()->second.as<string>(), parsedTop);
                }
                break;
            case(YAML::NodeType::Sequence):
                for(auto nodeName : apps[i].begin()->second){
                        ns3lxc::Application app(appName);
                    if(nodeName.Type() == YAML::NodeType::Scalar){
                        addAppToNode(&app, nodeName.as<string>(), parsedTop);
                    } else if(nodeName.Type() == YAML::NodeType::Map){
                        app.addCommand(nodeName.begin()->second.as<string>());
                        addAppToNode(&app, nodeName.begin()->first.as<string>(), parsedTop);
                    }
                }
                break;
            case(YAML::NodeType::Map):
            {
                parseMappedApplication(apps[i].begin()->second, appName, parsedTop);
                break;
            }
        }
    }
}