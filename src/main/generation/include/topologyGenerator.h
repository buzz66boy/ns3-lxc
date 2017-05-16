#ifndef __TOPOLOGY_GENERATOR_H_INCLUDED__
#define __TOPOLOGY_GENERATOR_H_INCLUDED__

#include <memory>
#include <map>
#include <string>

#include <lxc/lxccontainer.h>

#include "topology.h"

void generateTopology(ns3lxc::Topology *topology);

void findLinkTypesUsed(ns3lxc::Topology *topology);

#endif