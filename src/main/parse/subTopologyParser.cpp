#include <memory>
#include <vector>
#include <string>
#include <iostream>

#include "yaml-cpp/yaml.h"

#include "parserTags.h"
#include "topologyParser.h"
#include "subTopologyParser.h"

using namespace std;

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
                cout <<"PRPOGBGB" << endl;
            }
        }
        //parseTopology(node, &tempTop);
        //topPtr = shared_ptr<ns3lxc::Topology>(new ns3lxc::Topology(tempTop.topology));
        topList.push_back(topPtr);
        cout << "SIZE AGAIN " << topPtr->ifacesProvidedSubNames.size() << endl;
    }
    return topList;
}