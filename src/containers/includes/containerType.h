#ifndef __CONTAINER_TYPE_H_INCLUDED__
#define __CONTAINER_TYPE_H_INCLUDED__

#include <map>
#include <string>
#include <ostream>
#include <memory>

#include "node.h"

class ContainerType {
public:
    virtual void createContainer(std::shared_ptr<ns3lxc::Node>) = 0;
    virtual void startContainer(std::shared_ptr<ns3lxc::Node>) = 0; //Change to string?
    virtual void installApplications(std::shared_ptr<ns3lxc::Node>) = 0;
    virtual void runApplications(std::shared_ptr<ns3lxc::Node>) = 0; // for now, change to sync'ed packet later?
    virtual void grabOutput(std::shared_ptr<ns3lxc::Node>) = 0;
    virtual void teardownContainer(std::shared_ptr<ns3lxc::Node>) = 0;
};

#endif