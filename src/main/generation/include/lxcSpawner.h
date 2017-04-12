#ifndef __LXC_SPAWNER_H_INCLUDED__
#define __LXC_SPAWNER_H_INCLUDED__

#include <map>
#include <string>
#include <ostream>
#include <memory>

#include "topology.h"

class LXCSpawner {
private:
    static std::string defaultContainer = "ubuntu"
public:
    static void createContainers(ns3lxc::Topology *);
    static void startContainers(ns3lxc::Topology *); //Change to string?
    static void installApplications(ns3lxc::Topology *);
    static void runApplications(ns3lxc::Topology *); // for now, change to sync'ed packet later?
    static void grabOutput(ns3lxc::Topology *);
};

#endif