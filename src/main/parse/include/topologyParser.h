#ifndef __TOPOLOGY_PARSER_H_INCLUDED__
#define __TOPOLOGY_PARSER_H_INCLUDED__

// forward declared dependencies

// include dependencies
#include <string>
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
    ns3lxc::Topology topology;
	std::map<std::string, std::shared_ptr<ns3lxc::Topology> > includedTopologies;
	std::map<std::string, std::shared_ptr<ns3lxc::Node> > nodes;
	std::map<std::string, std::shared_ptr<ns3lxc::Link> > links;
	std::map<std::string, std::shared_ptr<ns3lxc::Application> > applications;
};

ns3lxc::Topology parseTopologyFile(std::string topPath);
void parseTopology(YAML::Node topology, ParsedTopology *parsedTop);
void renameSubTopologies(ns3lxc::Topology *topology, std::string prefix="");

#endif
