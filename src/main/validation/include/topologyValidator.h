#ifndef __TOPOLOGY_VALIDATOR_H_INCLUDED__
#define __TOPOLOGY_VALIDATOR_H_INCLUDED__

#include <memory>
#include "yntdl.h"

void validateNodeType(std::shared_ptr<yntdl::Node> nodePtr);
void validateLinkType(std::shared_ptr<yntdl::Link> linkPtr);
void validateTopologyTypes(yntdl::Topology *top);

#endif