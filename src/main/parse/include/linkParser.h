#ifndef __LINK_PARSER_H_INCLUDED__
#define __LINK_PARSER_H_INCLUDED__

#include <memory>

#include "link.h"
#include "topologyParser.h"

std::shared_ptr<ns3lxc::Link> parseLink(YAML::Node link, ParsedTopology *top);

#endif