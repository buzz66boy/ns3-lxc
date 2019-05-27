#ifndef __GENERIC_PACKMAN_H_INCLUDED__
#define __GENERIC_PACKMAN_H_INCLUDED__

#include <string>
#include <vector>
#include <memory>
#include <utility>

#include "yntdl.h"

#include "applicationType.h"

class GenericPackman : public ApplicationType {
private:
    std::string name;
    int sleepTime;
public:
    GenericPackman(std::string name, int sleepTime = 0): name(name), sleepTime(sleepTime) {};
    virtual bool isApplicationSynced() override {return true;};
    virtual InstallMethod getInstallMethod() override {return InstallMethod::PACKMAN;};
    virtual std::pair<std::string, std::string> getLocation(yntdl::Application *app) override {return std::pair<std::string, std::string>("/usr/bin/" + name, "");};
    virtual std::vector<std::pair<std::string, std::string> > getExistingRequiredFiles(yntdl::Application *app, std::shared_ptr<yntdl::Node> nodePtr) override {return std::vector<std::pair<std::string, std::string> >();};
    virtual std::vector<std::pair<std::string, std::string> > getRequiredFiles(yntdl::Application *app, std::shared_ptr<yntdl::Node> nodePtr) override {return std::vector<std::pair<std::string, std::string> >();};
    virtual std::vector<std::pair<std::string, bool> > getExecutionCommands(yntdl::Application *app, std::shared_ptr<yntdl::Node>) override;
    virtual std::vector<std::string> getCleanupLocations(yntdl::Application *app, std::shared_ptr<yntdl::Node>) override {return std::vector<std::string>();};
    virtual std::vector<std::string> getCleanupCommands(yntdl::Application *app, std::shared_ptr<yntdl::Node>) override {return std::vector<std::string>();};
};

#endif