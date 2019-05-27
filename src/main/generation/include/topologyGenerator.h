#ifndef __TOPOLOGY_GENERATOR_H_INCLUDED__
#define __TOPOLOGY_GENERATOR_H_INCLUDED__

#include <memory>
#include <map>
#include <string>

#include <lxc/lxccontainer.h>

#include "yntdl.h"

void generateTopology(yntdl::Topology *topology);

void findLinkTypesUsed(yntdl::Topology *topology);

#endif