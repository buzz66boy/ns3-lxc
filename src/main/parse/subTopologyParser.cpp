#include <memory>
#include <vector>
#include <string>
#include <iostream>

#include "yaml-cpp/yaml.h"

#include "parserTags.h"
#include "topologyParser.h"
#include "positionParser.h"
#include "subTopologyParser.h"

using namespace std;

static void applyIpOffset(string offset, ns3lxc::Topology *topology){
    for(auto linkPtr : topology->links){
        for(auto ifacePtr : linkPtr->ifaces){
            if(ifacePtr != nullptr && ifacePtr->ip != nullptr){
                ifacePtr->ip->applyOffset(offset);
            }
        }
    }
    for(auto topPtr : topology->subTopologies){
        applyIpOffset(offset, topPtr.get());
    }
}

std::vector<std::shared_ptr<ns3lxc::Topology> > parseSubTopology(YAML::Node node, ParsedTopology *top){
    size_t iters = 1;
    std::string origName = node.begin()->first.as<std::string>();
    node = node[origName];

    if(node[TAG_NUM]){
        iters = node[TAG_NUM].as<int>();
    }


    vector<shared_ptr<ns3lxc::Topology> > topList;
    for(size_t i = 0; i < iters; ++i){
        std::string name = origName;
        std::shared_ptr<ns3lxc::Topology> topPtr = nullptr;

        if(iters > 1){
            name += "_" + std::to_string(i + 1); //start indexing at 1
        }

        if(node[TAG_TEMPLATE]){
            string templateName = node[TAG_TEMPLATE].as<std::string>();
            if(top->includedTopologies.count(templateName) > 0){
                topPtr = make_shared<ns3lxc::Topology>(top->includedTopologies[templateName], name);
            } else {
                cerr << "The topology " + templateName + " used as a template was not found, check includes" << endl;
                exit(56);
            }
        }
        if(node[TAG_OFFSET]){
            string offset = node[TAG_OFFSET].as<std::string>();
            applyIpOffset(offset, topPtr.get());
        }
        if(node[TAG_POSITION]){
            if(iters > 1){
                if(node[TAG_POSITION][name]){
                    parsePositions(node[TAG_POSITION][name], topPtr.get());
                }
            } else {
                parsePositions(node[TAG_POSITION], topPtr.get());
            }
        } else if (node[pluralize(TAG_POSITION)]){
            if(iters > 1){
                YAML::Node baseNode = node[pluralize(TAG_POSITION)];
                if(baseNode[name]){
                    parsePositions(baseNode[name], topPtr.get());
                }
            } else {
                parsePositions(node[pluralize(TAG_POSITION)], topPtr.get());
            }
        }
        if(node[TAG_ROTATION]){
            if(node[TAG_ROTATION].Type() == YAML::NodeType::Scalar){
                applyRotation(node[TAG_ROTATION].as<int>(), topPtr.get());
            } else if(iters > 1){
                if(node[TAG_ROTATION][name]){
                    applyRotation(node[TAG_ROTATION][name].as<int>(), topPtr.get());
                }
            }
        } else if(node[pluralize(TAG_ROTATION)]){
            YAML::Node rotNode = node[pluralize(TAG_ROTATION)];
            if(rotNode.Type() == YAML::NodeType::Scalar){
                applyRotation(rotNode.as<int>(), topPtr.get());
            } else if(iters > 1){
                if(rotNode[name]){
                    applyRotation(rotNode[name].as<int>(), topPtr.get());
                }
            }
        }
        topList.push_back(topPtr);
    }
    for(auto topPtr : topList){
        for(auto nodePtr : topPtr->nodes){
            nodePtr->nodeNum = top->topology.curNodeNum++;
        }
    }
    return topList;
}