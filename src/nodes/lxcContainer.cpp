#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <cstring>

#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <lxc/lxccontainer.h>

#include "settingsParser.h"
#include "node.h"
#include "parserTags.h"
#include "errorCode.h"
#include "applicationTypeMap.h"
#include "lxcContainer.h"

using namespace std;

const std::map<std::string, std::map<std::string, std::string> >
packmanMap = {
    {"apt-get", {
        {"install", "install -y"},
        {"update", "update"},
        {"upgrade", "upgrade -y"},
        {"remove", "remove -y"}
    }},
    {"aptitude", {
        {"install", "install -y"},
        {"update", "update "},
        {"upgrade", "upgrade -y"},
        {"remove", "remove -y"}
    }},
    {"dnf", {
        {"install", "install -y"},
        {"update", "update"},
        {"upgrade", "upgrade -y"},
        {"remove", "remove -y"}
    }}, 
    {"pacman", {
        {"install", "--noconfirm -S"},
        {"update", "-Sy"},
        {"upgrade", "--noconfirm -Su"},
        {"remove", "--noconfirm -R"}
    }},
};

const std::map<std::string, std::string> 
distroPackMap = {
    {"ubuntu", "apt-get"},
    {"debian", "apt-get"},
    {"centos", "dnf"},
    {"fedora", "dnf"},
    {"redhat", "dnf"},
    {"archlinux", "pacman"},
};

/**
 * Helper Function for runCommandOnContainer
 **/
static int ex(void *arg){
    return system((char*)arg);
}

/**
 * Runs a command on the container using attach. Wait designates to wait for command completion before return
 * Returns: 
 *      wait==false: pid of process
 *      wait==true: exit status of process
 **/
static int runCommandOnContainer(string command, lxc_container *c, lxc_attach_options_t *opts, bool wait){

    char cmd[command.length() + 1];
    strcpy(cmd, command.c_str());
    int pid;
    c->attach(c, ex, cmd, opts, &pid);
    if(!wait){
        return pid;
    }
    int status;
    waitpid(pid, &status, 0);
    return status;
}

void LxcContainer::writeContainerConfig(std::shared_ptr<ns3lxc::Node> nodePtr, string configPath){
    std::ofstream ofs;
    ofs.open(configPath);

    for(auto it : nodePtr->ifaces){
        if(it.second.ip == nullptr || it.second.subnetMask == nullptr){
            continue;
        }
        ofs << "lxc.network.type = veth" << endl;
        ofs << "lxc.network.name = " << it.first << endl;
        ofs << "lxc.network.flags = up" << endl;
        ofs << "lxc.network.link = " << it.second.bridgeName << endl;
        ofs << "lxc.network.ipv4 = " << it.second.ip->str() << "/" << to_string(it.second.subnetMask->getCidr()) + " " << it.second.subnetMask->str() << endl;
        if(it.second.macAddr != ""){
            ofs << "lxc.network.hwaddr = " + it.second.macAddr << endl;
        } else {
            ofs << "lxc.network.hwaddr = xx:xx:xx:xx:xx:xx" << endl;
        }
    }
    ofs.close();
    configMap[nodePtr->name] = configPath;
}

/**
 * https://github.com/lxc/lxc/blob/master/src/lxc/lxccontainer.h
 */
void LxcContainer::createNode(std::shared_ptr<ns3lxc::Node> nodePtr) {
    lxc_container *c;
    containerMap[nodePtr->name] = shared_ptr<lxc_container>(lxc_container_new(nodePtr->name.c_str(), NULL));
    c = containerMap[nodePtr->name].get();
    if(!c){
        throw Ns3lxcException(ErrorCode::NODE_CREATE_FAILURE, nodePtr->name);
    }
    if(c->is_defined(c)){
        teardownNode(nodePtr);
        containerMap[nodePtr->name] = shared_ptr<lxc_container>(lxc_container_new(nodePtr->name.c_str(), NULL));
        c = containerMap[nodePtr->name].get();
    }
    string configPath = Settings::container_config_dir + "/" + nodePtr->name + ".conf";
    writeContainerConfig(nodePtr, configPath);

    if(!c->load_config(c, configMap[nodePtr->name].c_str())){
        throw Ns3lxcException(ErrorCode::NODE_CREATE_FAILURE, nodePtr->name);
    }
    if(!c->createl(c, "download", NULL, NULL, LXC_CREATE_QUIET, "-d", containerDistro.c_str(), "-r", containerRelease.c_str(), "-a", "amd64", NULL))
    {
        throw Ns3lxcException(ErrorCode::NODE_CREATE_FAILURE, nodePtr->name);
    }
    // c->set_config_item(c, );
    
}
void LxcContainer::startNode(std::shared_ptr<ns3lxc::Node> nodePtr) {
    lxc_container *c = containerMap[nodePtr->name].get();
    cout << "Starting container " + nodePtr->name << endl;
    if (!c->start(c, 0, NULL)) {
        //failed to start
        throw Ns3lxcException(ErrorCode::NODE_START_FAILURE, nodePtr->name);
    }
}

void LxcContainer::prepForInstall(std::vector<std::shared_ptr<ns3lxc::Application> > appList){
    // do nothing
}

void LxcContainer::installApplications(std::shared_ptr<ns3lxc::Node> nodePtr) {
    lxc_container *c = containerMap[nodePtr->name].get();
    lxc_attach_options_t opts = LXC_ATTACH_OPTIONS_DEFAULT;
    opts.namespaces = CLONE_NEWNS | CLONE_NEWPID; // Use host's network to retrieve packages (no net isolation)
    string packman = distroPackMap.at(containerDistro);
    string installCmd = packman + " " + packmanMap.at(packman).at("install") + " ";
    for(auto app : nodePtr->applications){
        if(applicationTypeMap.count(app.name) < 1 || applicationTypeMap.at(app.name)->getInstallMethod() == InstallMethod::PACKMAN){
            string command = installCmd + app.name;
            runCommandOnContainer(command, c, &opts, true);
        }
        if(applicationTypeMap.count(app.name) > 0){
            string lxcDir = "/var/lib/lxc/" + nodePtr->name + "/rootfs";
            ApplicationType *appType = applicationTypeMap.at(app.name);
            //Install
            switch(appType->getInstallMethod()){
                default:
                case(InstallMethod::BIN):
                case(InstallMethod::SCRIPT):
                case(InstallMethod::FOLDER):
                {
                    pair<string, string> location = appType->getLocation();
                    if(location.first != ""){
                        string localLoc = location.first;
                        string nodeLoc = (location.second != "") ? location.second : location.first;
                        if(nodeLoc[0] != '/'){
                            nodeLoc = "/" + nodeLoc;
                        }
                        system(("cp -r " + localLoc + " " + lxcDir + nodeLoc).c_str());
                    }
                    break;
                }
                case(InstallMethod::PACKMAN):
                    //already installed
                    break;
            }
            vector<pair<string, string> > existingFiles = appType->getExistingRequiredFiles(app.args, nodePtr);
            for(pair<string, string> locPair : existingFiles){
                //transfer existing files to location on container
                if(locPair.second[0] != '/'){
                    locPair.second = "/" + locPair.second;
                }
                system(("cp -r " + locPair.first + " " + lxcDir + locPair.second).c_str());
            }
            vector<pair<string, string> > requiredFiles = appType->getRequiredFiles(app.args, nodePtr);
            for(pair<string, string> locPair : requiredFiles){
                if(locPair.first[0] != '/'){
                    locPair.first = "/" + locPair.first;
                }
                string dest = lxcDir + locPair.first;
                ofstream nodeFile(dest);
                nodeFile << locPair.second;
                nodeFile.close();
            }
        }
    }
}

void LxcContainer::runApplications(std::shared_ptr<ns3lxc::Node> nodePtr) {
    lxc_container *c = containerMap[nodePtr->name].get();
    lxc_attach_options_t opts = LXC_ATTACH_OPTIONS_DEFAULT;
    for(auto app : nodePtr->applications){
        //Only run non-mapped app if args are present
        if(applicationTypeMap.count(app.name) < 1 && app.args != ""){
            string command = app.name + " " + app.args;
            int pid = runCommandOnContainer(command, c, &opts, false);
            pidMap[nodePtr->name].push_back(pid);
        }  else if(applicationTypeMap.count(app.name) > 0 && applicationTypeMap.at(app.name)->isApplicationSynced()) {
            switch(applicationTypeMap.at(app.name)->getInstallMethod()){
                default:
                case(InstallMethod::PACKMAN):
                    for(pair<string, bool> cmdPair : applicationTypeMap.at(app.name)->getExecutionCommands(app.args, nodePtr)){
                        string command = cmdPair.first;
                        int res = runCommandOnContainer(command, c, &opts, cmdPair.second);
                        if(!cmdPair.second){
                            pidMap[nodePtr->name].push_back(res);
                        }
                    }
                    break;
            }
        }
    }
}
void LxcContainer::grabOutput(std::shared_ptr<ns3lxc::Node> nodePtr) {
    for(auto app : nodePtr->applications){
        string lxcDir = "/var/lib/lxc/" + nodePtr->name + "/rootfs";
        if(applicationTypeMap.count(app.name) > 0){
            for(string path : applicationTypeMap.at(app.name)->getCleanupLocations(app.args, nodePtr)){
                if(path[0] != '/'){
                    path = '/' + path;
                }
                string outputLoc = Settings::top_output_dest;
                if(outputLoc[outputLoc.length() - 1] != '/'){
                    outputLoc = outputLoc + '/';
                }
                std::string filename = path.substr(path.find_last_of("\\/") + 1);
                filename = nodePtr->name + "_" + filename;
                system(("cp -r " + lxcDir + path + " " + outputLoc + filename).c_str());
            }
        }
    }
}

void LxcContainer::teardownNode(std::shared_ptr<ns3lxc::Node> nodePtr){
    lxc_container *c = containerMap[nodePtr->name].get();
    if(pidMap.count(nodePtr->name) > 0){
        for(int pid : pidMap.at(nodePtr->name)){
            int status;
            kill(pid, SIGTERM);
            waitpid(pid, &status, 0);
        }
        pidMap.erase(nodePtr->name);
    }
    if(Settings::run_mode == Mode::CLEANUP && c == nullptr){
        containerMap[nodePtr->name] = shared_ptr<lxc_container>(lxc_container_new(nodePtr->name.c_str(), NULL));
        c = containerMap[nodePtr->name].get();
    } else if (c == nullptr){
        cerr << "nullptr for container " + nodePtr->name << endl;
    }

    lxc_attach_options_t opts = LXC_ATTACH_OPTIONS_DEFAULT;
    for(auto app: nodePtr->applications){
        if(applicationTypeMap.count(app.name) > 0){
            for(string command : applicationTypeMap.at(app.name)->getCleanupCommands(app.args, nodePtr)){
                runCommandOnContainer(command, c, &opts, true);
            }
        }
    }
    if(c->is_defined(c)){
        //container exists, destroy
        c->stop(c);
        c->destroy(c);
    }
    c = nullptr;
    containerMap.erase(nodePtr->name);

}