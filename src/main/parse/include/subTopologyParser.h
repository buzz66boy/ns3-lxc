#ifndef __SUBTOPOLOGY_PARSER_H_INCLUDED__
#define __SUBTOPOLOGY_PARSER_H_INCLUDE

#include "yaml-cpp/yaml.h"

#include "topologyParser.h"

void parseSubtopology(YAML::Node node, ParsedTopology *top);

#endif