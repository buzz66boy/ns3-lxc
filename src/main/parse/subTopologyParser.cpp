#include <memory>
#include <vector>
#include <string>
#include <iostream>

#include "yaml-cpp/yaml.h"

#include "parserTags.h"
#include "errorCode.h"
#include "topologyParser.h"
#include "positionParser.h"
#include "subTopologyParser.h"

using namespace std;

static void applyIpOffset(string offset, ns3lxc::Topology *topology){
    // cout << "Apply " + offset + " to top " + topology->name << endl;
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
                throw Ns3lxcException(ErrorCode::TEMPLATE_NOT_FOUND, templateName);
            }
        }
        if(node[TAG_OFFSET] || node[pluralize(TAG_OFFSET)]){
            YAML::Node offsetNode = (node[TAG_OFFSET]) ? node[TAG_OFFSET] : node[pluralize(TAG_OFFSET)]; 
            if(offsetNode.Type() == YAML::NodeType::Scalar){
                string offset = offsetNode.as<std::string>();
                applyIpOffset(offset, topPtr.get());
            } else if (iters > 1 && offsetNode[name]){
                string offset = offsetNode[name].as<std::string>();
                applyIpOffset(offset, topPtr.get());
            }
        }
        if (node[TAG_POSITION] || node[pluralize(TAG_POSITION)]){
            YAML::Node baseNode = (node[TAG_POSITION]) ? node[TAG_POSITION] : node[pluralize(TAG_POSITION)];
            if(iters > 1 && baseNode[name]){
                parsePositions(baseNode[name], topPtr.get());
            } else if(baseNode.Type() == YAML::NodeType::Scalar){
                parsePositions(baseNode, topPtr.get());
            }
        }
        if(node[TAG_ROTATION] || node[pluralize(TAG_ROTATION)]){
            YAML::Node rotNode = (node[TAG_ROTATION]) ? node[TAG_ROTATION] : node[pluralize(TAG_ROTATION)];
            if(rotNode.Type() == YAML::NodeType::Scalar){
                applyRotation(rotNode.as<int>(), topPtr.get());
            } else if(iters > 1 && rotNode[name]){
                applyRotation(rotNode[name].as<int>(), topPtr.get());
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