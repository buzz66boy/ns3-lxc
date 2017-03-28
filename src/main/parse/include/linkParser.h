#ifndef __LINK_PARSER_H_INCLUDED__
#define __LINK_PARSER_H_INCLUDED__

#include "link.h"
#include "topologyParser.h"

ns3lxc::Link parseLink(YAML::Node link, ParsedTopology top);

#endif