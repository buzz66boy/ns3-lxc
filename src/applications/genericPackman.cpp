#include <string>
#include <vector>
#include <memory>

#include "genericPackman.h"

using namespace std;

std::vector<std::string> GenericPackman::getExecutionCommands(std::string args, std::shared_ptr<ns3lxc::Node> nodePtr){
    string build = "";
    if(sleepTime > 0){
        build = build + "sleep " + to_string(sleepTime) + ";";
    }
    build = build + name + " " + args + " &> " + nodePtr->name + "_" + name;
    return vector<string>({build});
}
std::vector<std::string> GenericPackman::getCleanupLocations(std::string args, std::shared_ptr<ns3lxc::Node> nodePtr){
    return std::vector<string> ({nodePtr->name + "_" + name});
}