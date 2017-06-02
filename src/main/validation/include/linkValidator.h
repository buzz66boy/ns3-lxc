#ifndef __LINK_VALIDATOR_H_INCLUDED__
#define __LINK_VALIDATOR_H_INCLUDED__

#include <memory>

#include "link.h"

void validateLink(std::shared_ptr<ns3lxc::Link> linkPtr);

#endif