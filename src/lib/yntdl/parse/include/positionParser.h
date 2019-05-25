#ifndef __POSITION_PARSER_H_INCLUDED__
#define __POSITION_PARSER_H_INCLUDED__

#include <memory>

#include "yaml-cpp/yaml.h"
#include "position.h"
#include "node.h"
#include "topology.h"

void parsePositions(YAML::Node posNode, std::shared_ptr<yntdl::Node> nodePtr);
void parsePositions(YAML::Node posNode, yntdl::Topology *topPtr);
void applyRotation(int rotation, yntdl::Topology *topPtr);
void computeAbsolutePositions(yntdl::Topology *top);

#endif