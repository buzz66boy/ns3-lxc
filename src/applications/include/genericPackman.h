#ifndef __GENERIC_PACKMAN_H_INCLUDED__
#define __GENERIC_PACKMAN_H_INCLUDED__

#include <string>
#include <vector>
#include <memory>

#include "node.h"
#include "topology.h"

#include "applicationType.h"

class GenericPackman : public ApplicationType {
private:
    std::string name;
    int sleepTime;
public:
    GenericPackman(std::string name, int sleepTime = 0): name(name), sleepTime(sleepTime) {};
    virtual bool isApplicationSynced() override {return true;};
    virtual InstallMethod getInstallMethod() override {return InstallMethod::PACKMAN;};
    virtual std::string getLocation() override {return "/usr/bin/" + name;};
    virtual std::string getConfigFilename(std::string args, std::shared_ptr<ns3lxc::Node>) override {return "";};
    virtual std::string writeConfig(std::string args, std::shared_ptr<ns3lxc::Node>, std::ostream) override {return "";};
    virtual std::vector<std::string> getExecutionCommands(std::string args, std::shared_ptr<ns3lxc::Node>) override;
    virtual std::vector<std::string> getCleanupLocations(std::string args, std::shared_ptr<ns3lxc::Node>) override;
    virtual std::vector<std::string> getCleanupCommands(std::string args, std::shared_ptr<ns3lxc::Node>) override {return std::vector<std::string>();};
};

#endif