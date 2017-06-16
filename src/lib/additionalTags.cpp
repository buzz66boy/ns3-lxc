#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <algorithm>
#include <string>

#include "yaml-cpp/yaml.h"

#include "additionalTags.h"

using namespace ns3lxc;

void AdditionalTags::mapAdditionalTags(std::vector<std::string> knownTags, YAML::Node mapRoot){
    if(mapRoot.Type() != YAML::NodeType::Map){
        std::cerr << "Additional Tag Root is not a Map Type" << std::endl;
        return;
    }
    for(auto pair: mapRoot){
        std::string node = pair.first.as<std::string>();
        if(find(knownTags.begin(), knownTags.end(), node) != knownTags.end()){
            continue;
        }
        std::cout << "Adding additional flag: " + node << std::endl;
        additionalTags[node] = pair.second;
    }
}