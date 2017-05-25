#ifndef __QUAGGA_H_INCLUDED__
#define __QUAGGA_H_INCLUDED__

#include <vector>
#include <string>
#include <utility>

#include "genericPackman.h"

class Quagga : public GenericPackman {
public:
    Quagga(): GenericPackman("quagga", 0) {}
    virtual std::vector<std::pair<std::string, std::string> > getExistingRequiredFiles(std::string args, std::shared_ptr<ns3lxc::Node> nodePtr) override;
    virtual std::vector<std::pair<std::string, std::string> > getRequiredFiles(std::string args, std::shared_ptr<ns3lxc::Node> nodePtr) override;
    virtual std::vector<std::pair<std::string, bool> > getExecutionCommands(std::string args, std::shared_ptr<ns3lxc::Node>) override;
    virtual std::vector<std::string> getCleanupLocations(std::string args, std::shared_ptr<ns3lxc::Node>) override;
    virtual std::vector<std::string> getCleanupCommands(std::string args, std::shared_ptr<ns3lxc::Node>) override;
};

#endif