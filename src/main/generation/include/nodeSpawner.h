#ifndef __NODE_SPAWNER_H_INCLUDED__
#define __NODE_SPAWNER_H_INCLUDED__

#include <map>
#include <string>
#include <ostream>
#include <memory>

#include "topology.h"

class NodeSpawner {
public:
    static void createNodes(ns3lxc::Topology *);
    static void startNodes(ns3lxc::Topology *); //Change to string?
    static void installApplications(ns3lxc::Topology *);
    static void runCommands(ns3lxc::Topology *);
    static void runApplications(ns3lxc::Topology *); // for now, change to sync'ed packet later?
    static void grabOutput(ns3lxc::Topology *);
    static void teardownNodes(ns3lxc::Topology *);
};

#endif