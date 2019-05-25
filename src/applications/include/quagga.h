#ifndef __QUAGGA_H_INCLUDED__
#define __QUAGGA_H_INCLUDED__

#include <vector>
#include <string>
#include <utility>

#include "genericPackman.h"

class Quagga : public GenericPackman {
public:
    Quagga(): GenericPackman("quagga", 0) {}
    virtual std::vector<std::pair<std::string, std::string> > getExistingRequiredFiles(yntdl::Application *app, std::shared_ptr<yntdl::Node> nodePtr) override;
    virtual std::vector<std::pair<std::string, std::string> > getRequiredFiles(yntdl::Application *app, std::shared_ptr<yntdl::Node> nodePtr) override;
    virtual std::vector<std::pair<std::string, bool> > getExecutionCommands(yntdl::Application *app, std::shared_ptr<yntdl::Node>) override;
    virtual std::vector<std::string> getCleanupLocations(yntdl::Application *app, std::shared_ptr<yntdl::Node>) override;
    virtual std::vector<std::string> getCleanupCommands(yntdl::Application *app, std::shared_ptr<yntdl::Node>) override;
};

#endif