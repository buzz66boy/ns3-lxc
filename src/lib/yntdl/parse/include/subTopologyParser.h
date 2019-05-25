#ifndef __SUBTOPOLOGY_PARSER_H_INCLUDED__
#define __SUBTOPOLOGY_PARSER_H_INCLUDE

#include <memory>

#include "yaml-cpp/yaml.h"

#include "topology.h"
#include "topologyParser.h"

std::vector<std::shared_ptr<yntdl::Topology> > parseSubTopology(YAML::Node node, ParsedTopology *top);

#endif