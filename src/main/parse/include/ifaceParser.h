#ifndef __IFACE_PARSER_H_INCLUDED__
#define __IFACE_PARSER_H_INCLUDED__

#include <string>
#include <memory>

#include "yaml-cpp/yaml.h"
#include "topologyParser.h"

void parseIfacesProvided(YAML::Node ifaces, ParsedTopology *parsedTop);

void parseAcceptedIfaces(YAML::Node acceptedIface, ParsedTopology *top);

void parseIfacesAccepted(YAML::Node ifacesAccepted, ParsedTopology *parsedTop);

#endif