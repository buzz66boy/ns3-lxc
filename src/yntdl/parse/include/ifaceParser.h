#ifndef __IFACE_PARSER_H_INCLUDED__
#define __IFACE_PARSER_H_INCLUDED__

#include <string>
#include <memory>

#include "yaml-cpp/yaml.h"
#include "topology.h"
#include "topologyParser.h"

std::weak_ptr<ns3lxc::IfaceProvider> getProvider(std::string provider, ns3lxc::Topology *top);

std::weak_ptr<ns3lxc::IfaceAcceptor> getAcceptor(std::string acceptor, ns3lxc::Topology *top);

void parseIfacesProvided(YAML::Node ifaces, ParsedTopology *parsedTop);

void parseAcceptedIfaces(YAML::Node acceptedIface, ParsedTopology *top);

void parseIfacesAccepted(YAML::Node ifacesAccepted, ParsedTopology *parsedTop);

#endif