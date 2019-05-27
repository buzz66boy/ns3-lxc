#ifndef __LXC_CONTAINER_H_INCLUDED__
#define __LXC_CONTAINER_H_INCLUDED__

#include <string>
#include <map>
#include <memory>
#include <vector>

#include <lxc/lxccontainer.h>

#include "yntdl.h"
#include "nodeType.h"

class LxcContainer : public NodeType {
    std::string containerDistro;
    std::string containerRelease;

    std::map<std::string, std::shared_ptr<lxc_container> > containerMap;
    std::map<std::string, std::string> configMap;
    std::map<std::string, std::vector<int> > pidMap;

    void writeContainerConfig(std::shared_ptr<yntdl::Node> nodePtr, std::string configPath);
public:
    LxcContainer(std::string distro, std::string release): containerDistro(distro), containerRelease(release) {};

    bool createBridgesTaps() override { return true; }
    void createNode(std::shared_ptr<yntdl::Node>) override;
    void startNode(std::shared_ptr<yntdl::Node>) override;
    void prepForInstall(std::vector<std::shared_ptr<yntdl::Application> > appList) override;
    void installApplications(std::shared_ptr<yntdl::Node>) override;
    void runCommands(std::shared_ptr<yntdl::Node>) override;
    void runApplications(std::shared_ptr<yntdl::Node>) override;
    void grabOutput(std::shared_ptr<yntdl::Node>) override;
    void teardownNode(std::shared_ptr<yntdl::Node>) override;
};

#endif