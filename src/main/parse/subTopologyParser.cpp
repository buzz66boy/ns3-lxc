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
            ifacePtr->ip->applyOffset(offset);
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
        ParsedTopology tempTop;

        if(node[TAG_TEMPLATE]){
            string templateName = node[TAG_TEMPLATE].as<std::string>();
            cout << "template: " << templateName << endl;
            if(top->includedTopologies.count(templateName) > 0){
                //tempTop.topology = ns3lxc::Topology(top->includedTopologies[templateName], name);
                topPtr = shared_ptr<ns3lxc::Topology>(new ns3lxc::Topology(top->includedTopologies[templateName], name));
            } else {
                cout <<"PROB" << endl;
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
        topList.push_back(topPtr);
    }
    for(auto topPtr : topList){
        for(auto nodePtr : topPtr->nodes){
            nodePtr->nodeNum = top->topology.curNodeNum++;
        }
    }
    return topList;
}