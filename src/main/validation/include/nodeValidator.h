#ifndef __NODE_VALIDATOR_H_INCLUDED__
#define __NODE_VALIDATOR_H_INCLUDED__

#include <memory>
#include "node.h"

void validateNode(std::shared_ptr<ns3lxc::Node> nodePtr);

#endif