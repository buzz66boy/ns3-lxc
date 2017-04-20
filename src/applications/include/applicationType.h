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
    virtual bool isApplicationSynced() = 0;
    virtual std::string getConfigLocation(ns3lxc::Node, ns3lxc::Topology) = 0;
    virtual void writeConfig(ns3lxc::Node, ns3lxc::Topology, std::ostream) = 0;
    virtual void startExecution(ns3lxc::Node, ns3lxc::Topology, std::ostream) = 0;
    virtual void cleanup(ns3lxc::Node, ns3lxc::Topology, std::ostream) = 0;
};

#endif