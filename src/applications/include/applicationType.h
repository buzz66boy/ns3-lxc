#ifndef __APPLICATION_TYPE_H_INCLUDED__
#define __APPLICATION_TYPE_H_INCLUDED__

#include <map>
#include <string>
#include <ostream>
#include <memory>
#include <vector>

#include "node.h"
#include "topology.h"

enum class InstallMethod {PACKMAN, BIN, FOLDER, SCRIPT};

class ApplicationType {
public:
    /**
     * Denotes whether the application should start with the simulation or before
     *
     **/
    virtual bool isApplicationSynced() = 0;
    virtual InstallMethod getInstallMethod() = 0;
    virtual std::string getLocation() = 0;
    virtual std::string getConfigFilename(std::string args, std::shared_ptr<ns3lxc::Node> nodePtr) = 0;
    virtual std::string writeConfig(std::string args, std::shared_ptr<ns3lxc::Node> nodePtr, std::ostream) = 0;
    virtual std::vector<std::string> getExecutionCommands(std::string args, std::shared_ptr<ns3lxc::Node> nodePtr) = 0;
    virtual std::vector<std::string> getCleanupLocations(std::string args, std::shared_ptr<ns3lxc::Node> nodePtr) = 0;
    virtual std::vector<std::string> getCleanupCommands(std::string args, std::shared_ptr<ns3lxc::Node> nodePtr) = 0;
};

#endif