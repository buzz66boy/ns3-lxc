#ifndef __NODE_SPAWNER_H_INCLUDED__
#define __NODE_SPAWNER_H_INCLUDED__

#include <map>
#include <string>
#include <ostream>
#include <memory>

#include "yntdl.h"

class NodeSpawner {
public:
    static void createNodes(yntdl::Topology *);
    static void startNodes(yntdl::Topology *); //Change to string?
    static void installApplications(yntdl::Topology *);
    static void runCommands(yntdl::Topology *);
    static void runApplications(yntdl::Topology *); // for now, change to sync'ed packet later?
    static void grabOutput(yntdl::Topology *);
    static void teardownNodes(yntdl::Topology *);
};

#endif