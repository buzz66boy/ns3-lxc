#include <vector>
#include <string>

#include "quagga.h"

std::string Quagga::getConfigFilename(std::string args, std::shared_ptr<ns3lxc::Node>){
    return "";
}
std::string Quagga::writeConfig(std::string args, std::shared_ptr<ns3lxc::Node>, std::ostream){
    
    return "";
}
std::vector<std::string> Quagga::getExecutionCommands(std::string args, std::shared_ptr<ns3lxc::Node>){
    return std::vector<std::string>();
}
std::vector<std::string> Quagga::getCleanupLocations(std::string args, std::shared_ptr<ns3lxc::Node>){
    return std::vector<std::string>();
}
std::vector<std::string> Quagga::getCleanupCommands(std::string args, std::shared_ptr<ns3lxc::Node>){
    return std::vector<std::string>();
}
