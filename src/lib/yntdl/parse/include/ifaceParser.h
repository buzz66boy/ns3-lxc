#ifndef __IFACE_PARSER_H_INCLUDED__
#define __IFACE_PARSER_H_INCLUDED__

#include <string>
#include <memory>

#include "yaml-cpp/yaml.h"
#include "topology.h"
#include "topologyParser.h"

std::weak_ptr<yntdl::IfaceProvider> getProvider(std::string provider, yntdl::Topology *top);

std::weak_ptr<yntdl::IfaceAcceptor> getAcceptor(std::string acceptor, yntdl::Topology *top);

void parseIfacesProvided(YAML::Node ifaces, ParsedTopology *parsedTop);

void parseAcceptedIfaces(YAML::Node acceptedIface, ParsedTopology *top);

void parseIfacesAccepted(YAML::Node ifacesAccepted, ParsedTopology *parsedTop);

#endif