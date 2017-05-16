#ifndef __POSITION_PARSER_H_INCLUDED__
#define __POSITION_PARSER_H_INCLUDED__

#include <memory>

#include "yaml-cpp/yaml.h"
#include "position.h"
#include "node.h"
#include "topology.h"

void parsePositions(YAML::Node posNode, std::shared_ptr<ns3lxc::Node> nodePtr);
void parsePositions(YAML::Node posNode, ns3lxc::Topology *topPtr);
void computeAbsolutePositions(ns3lxc::Topology *top);

#endif