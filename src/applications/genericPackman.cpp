#include <string>
#include <vector>
#include <memory>

#include "genericPackman.h"

using namespace std;

std::vector<std::pair<std::string, bool> > GenericPackman::getExecutionCommands(std::string args, std::shared_ptr<ns3lxc::Node> nodePtr){
    string build = "";
    if(sleepTime > 0){
        build = build + "sleep " + to_string(sleepTime) + ";";
    }
    build = build + name + " " + args + " &> " + nodePtr->name + "_" + name;
    return vector<pair<string, bool> >({pair<string, bool>(build, false)});
}