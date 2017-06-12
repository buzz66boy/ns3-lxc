#include <string>
#include <vector>
#include <memory>

#include "genericPackman.h"

using namespace std;

std::vector<std::pair<std::string, bool> > GenericPackman::getExecutionCommands(ns3lxc::Application *app, std::shared_ptr<ns3lxc::Node> nodePtr){
    vector<pair<string, bool> > cmdVect;
    if(app->commands.size() > 0){
        for(size_t i = 0; i < app->commands.size(); ++i){
            if(i == app->commands.size() - 1){
                string build = "";
                if(sleepTime > 0){
                    build = build + "sleep " + to_string(sleepTime) + ";";
                }
                build = build + app->commands[i].first + " &> " + nodePtr->name + "_" + name + "_log.txt";
            } else {

            }
        }
    }
    return cmdVect;
}