#ifndef __LXC_CONTAINER_TYPE_H_INCLUDED__
#define __LXC_CONTAINER_TYPE_H_INCLUDED__

#include <string>
#include <map>
#include <memory>

#include <lxc/lxccontainer.h>

#include "node.h"
#include "containerType.h"

class LxcContainerType : public ContainerType {
    std::string containerDistro;
    std::string containerRelease;

    std::map<std::string, std::shared_ptr<lxc_container> > containerMap;
public:
    LxcContainerType(std::string distro, std::string release): containerDistro(distro), containerRelease(release) {};

    void createContainer(std::shared_ptr<ns3lxc::Node>) override;
    void startContainer(std::shared_ptr<ns3lxc::Node>) override;
    void installApplications(std::shared_ptr<ns3lxc::Node>) override;
    void runApplications(std::shared_ptr<ns3lxc::Node>) override;
    void grabOutput(std::shared_ptr<ns3lxc::Node>) override;
    void teardownContainer(std::shared_ptr<ns3lxc::Node>) override;
};

#endif