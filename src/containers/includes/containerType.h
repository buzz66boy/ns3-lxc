#ifndef __CONTAINER_TYPE_H_INCLUDED__
#define __CONTAINER_TYPE_H_INCLUDED__

#include <map>
#include <string>
#include <ostream>
#include <memory>

#include "node.h"

class ContainerType {
public:
    virtual void createContainer(ns3lxc::Node);
    virtual void startContainer(ns3lxc::Node); //Change to string?
    virtual void installApplications(ns3lxc::Node);
    virtual void runApplications(ns3lxc::Node); // for now, change to sync'ed packet later?
    virtual void grabOutput(ns3lxc::Node);
};

#endif