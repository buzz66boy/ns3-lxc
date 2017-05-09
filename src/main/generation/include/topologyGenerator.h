#ifndef __TOPOLOGY_GENERATOR_H_INCLUDED__
#define __TOPOLOGY_GENERATOR_H_INCLUDED__

#include <memory>
#include <map>
#include <string>

#include <lxc/lxccontainer.h>

#include "topology.h"

// class GeneratedTopology {
// public:
//     ns3lxc::Topology *topPtr;

//     std::map<std::string, std::shared_ptr<lxc_container> > containerMap;

//     std::string tempDir;

//     GeneratedTopology(ns3lxc::Topology *topology): topPtr(topology) {}
// };

void generateTopology(ns3lxc::Topology *topology);

void spawnTopology(ns3lxc::Topology *topology);

void despawnTopology(ns3lxc::Topology *topology);

void findLinkTypesUsed(ns3lxc::Topology *topology);

#endif