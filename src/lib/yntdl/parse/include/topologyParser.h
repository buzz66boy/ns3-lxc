#ifndef __TOPOLOGY_PARSER_H_INCLUDED__
#define __TOPOLOGY_PARSER_H_INCLUDED__

// forward declared dependencies

// include dependencies
#include <string>
#include <vector>
#include <map>
#include <memory>

#include "yaml-cpp/yaml.h"

#include "topology.h"
#include "node.h"
#include "link.h"
#include "application.h"

// declarations

class ParsedTopology {
public:
    yntdl::Topology topology;
	std::map<std::string, std::shared_ptr<yntdl::Topology> > includedTopologies;
	std::map<std::string, std::shared_ptr<yntdl::Node> > nodes;
	std::map<std::string, std::shared_ptr<yntdl::Link> > links;
	std::map<std::string, std::shared_ptr<yntdl::Application> > applications;
};

yntdl::Topology parseTopologyFile(std::string topPath);
void parseTopology(YAML::Node topology, ParsedTopology *parsedTop);
void renameSubTopologies(yntdl::Topology *topology, std::string prefix="");
std::shared_ptr<yntdl::Node> findNode(std::vector<std::string> search, yntdl::Topology *top);

#endif
