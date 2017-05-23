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
#include "applicationTypeMap.h"
#include "lxcContainerType.h"

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


void LxcContainerType::writeContainerConfig(std::shared_ptr<ns3lxc::Node> nodePtr, string configPath){
    std::ofstream ofs;
    ofs.open(configPath);


    // ofs << "lxc.network.type = macvlan" << endl;
    // ofs << "lxc.network.name = extacc" << endl;
    // ofs << "lxc.network.macvlan.mode = bridge" << endl;
    // ofs << "lxc.network.flags = up" << endl;
    // ofs << "lxc.network.link = mac0" << endl;

    // ofs << "lxc.network.type = veth" << endl;
    // ofs << "lxc.network.name = extacc" << endl;
    // ofs << "lxc.network.link = lxcbr0" << endl;
    // ofs << "lxc.network.hwaddr = 00:16:3e:xx:xx:xx" << endl;
    // ofs << "lxc.network.flags = up" << endl;

    for(auto it : nodePtr->ifaces){
        if(it.second.ip == nullptr || it.second.subnetMask == nullptr){
            continue;
        }
        ofs << "lxc.network.type = veth" << endl;
        ofs << "lxc.network.name = " << it.first << endl;
        ofs << "lxc.network.flags = up" << endl;
        ofs << "lxc.network.link = " << it.second.bridgeName << endl;
        ofs << "lxc.network.ipv4 = " << it.second.ip->str() << "/" << to_string(it.second.subnetMask->getCidr()) + " " << it.second.subnetMask->str() << endl;
        ofs << "lxc.network.hwaddr = xx:xx:xx:xx:xx:xx" << endl;
    }

    ofs.close();
    configMap[nodePtr->name] = configPath;
}

/**
 * https://github.com/lxc/lxc/blob/master/src/lxc/lxccontainer.h
 */
void LxcContainerType::createContainer(std::shared_ptr<ns3lxc::Node> nodePtr) {
    lxc_container *c;
    containerMap[nodePtr->name] = shared_ptr<lxc_container>(lxc_container_new(nodePtr->name.c_str(), NULL));
    c = containerMap[nodePtr->name].get();
    if(!c){
        cerr << "PROBLEMS WITH CONTAINER" << endl;
    }
    if(c->is_defined(c)){
        teardownContainer(nodePtr);
        containerMap[nodePtr->name] = shared_ptr<lxc_container>(lxc_container_new(nodePtr->name.c_str(), NULL));
        c = containerMap[nodePtr->name].get();
    }
    string configPath = Settings::container_config_dir + "/" + nodePtr->name + ".conf";
    writeContainerConfig(nodePtr, configPath);

    if(!c->load_config(c, configMap[nodePtr->name].c_str())){
        cerr << "EMERGENCY" << endl;
    }
    if(!c->createl(c, "download", NULL, NULL, LXC_CREATE_QUIET, "-d", containerDistro.c_str(), "-r", containerRelease.c_str(), "-a", "amd64", NULL))
    {
        //create failed
        cerr << "Container " << nodePtr->name << " could not be created" << endl;
    }
    // c->set_config_item(c, );
    
}
void LxcContainerType::startContainer(std::shared_ptr<ns3lxc::Node> nodePtr) {
    lxc_container *c = containerMap[nodePtr->name].get();
    cout << "Starting container " + nodePtr->name << endl;
    if (!c->start(c, 0, NULL)) {
        //failed to start
        cerr << "Container " << nodePtr->name << " failed to start" << endl;
    }
}

void LxcContainerType::prepForInstall(std::vector<std::shared_ptr<ns3lxc::Application> > appList){
    for(auto appPtr : appList){
        if(applicationTypeMap.count(appPtr->name) > 0){

        }
    }
}

static int ex(void *arg){
    return system((char*)arg);
}

void LxcContainerType::installApplications(std::shared_ptr<ns3lxc::Node> nodePtr) {
    lxc_container *c = containerMap[nodePtr->name].get();
    lxc_attach_options_t opts = LXC_ATTACH_OPTIONS_DEFAULT;
    opts.namespaces = CLONE_NEWNS | CLONE_NEWPID;
    for(auto app : nodePtr->applications){
        if(applicationTypeMap.count(app.name) > 0){
            string configName = applicationTypeMap.at(app.name)->getConfigFilename(app.args, nodePtr);
        }
    }
    string packman = distroPackMap.at(containerDistro);
    string installCmd = packman + " " + packmanMap.at(packman).at("install") + " ";
    for(auto app : nodePtr->applications){
        //FIXME bad idea (relying on set amt of args)
        if(applicationTypeMap.count(app.name) < 1 || applicationTypeMap.at(app.name)->getInstallMethod() == InstallMethod::PACKMAN){
            string command = installCmd + app.name;
            char cmd[command.length() + 1];
            strcpy(cmd, command.c_str());
            int pid, status;
            c->attach(c, ex, cmd, &opts, &pid);
            waitpid(pid, &status, 0);
        }
    }
}

void LxcContainerType::runApplications(std::shared_ptr<ns3lxc::Node> nodePtr) {
    lxc_container *c = containerMap[nodePtr->name].get();
    lxc_attach_options_t opts = LXC_ATTACH_OPTIONS_DEFAULT;
    for(auto app : nodePtr->applications){
        if(applicationTypeMap.count(app.name) < 1){
            int pid;
            char cmd[app.name.length() + 2 + app.args.length()];
            strcpy(cmd, (app.name + " " + app.args).c_str());
            int res = c->attach(c, ex, cmd, &opts, &pid);
            pidMap[nodePtr->name].push_back(pid);
        } else {
            switch(applicationTypeMap.at(app.name)->getInstallMethod()){
                default:
                case(InstallMethod::PACKMAN):
                    for(string command : applicationTypeMap.at(app.name)->getExecutionCommands(app.args, nodePtr)){
                        char cmd[command.length() + 1];
                        strcpy(cmd, command.c_str());
                        int pid;
                        int res = c->attach(c, ex, cmd, &opts, &pid);
                        pidMap[nodePtr->name].push_back(pid);
                    }
                    break;
            }
        }
    }
}
void LxcContainerType::grabOutput(std::shared_ptr<ns3lxc::Node> nodePtr) {

}

void LxcContainerType::teardownContainer(std::shared_ptr<ns3lxc::Node> nodePtr){
    lxc_container *c = containerMap[nodePtr->name].get();
    if(pidMap.count(nodePtr->name) > 0){
        lxc_attach_options_t opts = LXC_ATTACH_OPTIONS_DEFAULT;
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
    if(c->is_defined(c)){
        //container exists, destroy
        c->stop(c);
        c->destroy(c);
    }
    c = nullptr;
    containerMap.erase(nodePtr->name);

}