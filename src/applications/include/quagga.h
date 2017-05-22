#ifndef __QUAGGA_H_INCLUDED__
#define __QUAGGA_H_INCLUDED__

#include "genericPackman.h"

class Quagga : public GenericPackman {
public:
    Quagga(): GenericPackman("quagga", 2) {}
    virtual std::string getConfigFilename(std::string args, std::shared_ptr<ns3lxc::Node>) override;
    virtual std::string writeConfig(std::string args, std::shared_ptr<ns3lxc::Node>, std::ostream) override;
    virtual std::vector<std::string> getExecutionCommands(std::string args, std::shared_ptr<ns3lxc::Node>) override;
    virtual std::vector<std::string> getCleanupLocations(std::string args, std::shared_ptr<ns3lxc::Node>) override;
    virtual std::vector<std::string> getCleanupCommands(std::string args, std::shared_ptr<ns3lxc::Node>) override;
};

#endif