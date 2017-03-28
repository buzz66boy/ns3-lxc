#ifndef __NODE_PARSER_H_INCLUDED__
#define __NODE_PARSER_H_INCLUDED__

// forward declared dependencies

// include dependencies
#include <vector>

#include "node.h"
#include "topologyParser.h"

// declarations

std::vector<ns3lxc::Node> parseNode(YAML::Node node, ParsedTopology top);

#endif
