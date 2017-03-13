#ifndef __NODE_PARSER_H_INCLUDED__
#define __NODE_PARSER_H_INCLUDED__

// forward declared dependencies

// include dependencies
#include <string>
#include <vector>
#include <map>

#include <arpa/inet.h>

#include "topology.h"
#include "node.h"
#include "application.h"
#include "topologyParser.h"

// declarations

class ParsedNode {
public:
    ns3lxc::Node node;
    std::string templateName;

    ParsedNode(YAML::Node node);
};

#endif
