#ifndef __APPLICATION_TYPE_H_INCLUDED__
#define __APPLICATION_TYPE_H_INCLUDED__

#include <map>
#include <string>
#include <ostream>

#include "node.h"
#include "topology.h"

class ApplicationType {
    virtual ~ApplicationType() = 0;
public:
    /**
     * Denotes whether the application should start with the simulation or before
     *
     **/
    bool isApplicationSynced();
    std::string getConfigLocation(ns3lxc::Node, ns3lxc::Topology);
    void writeConfig(ns3lxc::Node, ns3lxc::Topology, std::ostream);
    void startExecution(ns3lxc::Node, ns3lxc::Topology, std::ostream);
    void cleanup(ns3lxc::Node, ns3lxc::Topology, std::ostream);

};

#endif