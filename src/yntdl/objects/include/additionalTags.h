#ifndef __ADDITIONAL_TAGS_H_INCLUDED__
#define __ADDITIONAL_TAGS_H_INCLUDED__

#include <map>
#include <string>
#include <vector>

#include "yaml-cpp/yaml.h"

namespace ns3lxc {

class AdditionalTags {
public:
    std::map<std::string, YAML::Node> additionalTags;

    /**
     * Adds unrecognized tags to the additionalTags map
     **/
    void mapAdditionalTags(std::vector<std::string> knownTags, YAML::Node mapRoot);
};

};

#endif