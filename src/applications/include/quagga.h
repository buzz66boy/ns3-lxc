#ifndef __QUAGGA_H_INCLUDED__
#define __QUAGGA_H_INCLUDED__

#include <vector>
#include <string>
#include <utility>

#include "genericPackman.h"

class Quagga : public GenericPackman {
public:
    Quagga(): GenericPackman("quagga", 0) {}
    virtual std::vector<std::pair<std::string, std::string> > getExistingRequiredFiles(ns3lxc::Application *app, std::shared_ptr<ns3lxc::Node> nodePtr) override;
    virtual std::vector<std::pair<std::string, std::string> > getRequiredFiles(ns3lxc::Application *app, std::shared_ptr<ns3lxc::Node> nodePtr) override;
    virtual std::vector<std::pair<std::string, bool> > getExecutionCommands(ns3lxc::Application *app, std::shared_ptr<ns3lxc::Node>) override;
    virtual std::vector<std::string> getCleanupLocations(ns3lxc::Application *app, std::shared_ptr<ns3lxc::Node>) override;
    virtual std::vector<std::string> getCleanupCommands(ns3lxc::Application *app, std::shared_ptr<ns3lxc::Node>) override;
};

#endif