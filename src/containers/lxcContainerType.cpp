#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>

#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <lxc/lxccontainer.h>

#include "settingsParser.h"
#include "node.h"
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
    {"arch", "pacman"},
};


void LxcContainerType::writeContainerConfig(std::shared_ptr<ns3lxc::Node> nodePtr, string configPath){
    std::ofstream ofs;
    ofs.open(configPath);

    for(auto it : nodePtr->ifaces){
        if(it.second->ip == nullptr || it.second->subnetMask == nullptr){
            continue;
        }
        ofs << "lxc.network.type = veth" << endl;
        ofs << "lxc.network.name = " << it.first << endl;
        ofs << "lxc.network.flags = up" << endl;
        ofs << "lxc.network.link = " << it.second->bridgeName << endl;
        //FIXME: change to using right subnet
        ofs << "lxc.network.ipv4 = " << it.second->ip->str() << "/24 " << it.second->subnetMask->str() << endl;
        ofs << "lxc.network.hwaddr = xx:xx:xx:xx:xx:xx" << endl;
    }
    // ofs << "lxc.network.type = veth" << endl;
    // ofs << "lxc.network.name = extacc" << endl;
    // ofs << "lxc.network.link = lxcbr0" << endl;
    // ofs << "lxc.network.flags = up" << endl;
    // ofs << "lxc.network.hwaddr = 00:16:3e:xx:xx:xx" << endl;
    
    // file.write("lxc.network.type = veth\n")
    // file.write("lxc.network.flags = up\n")
    // file.write("lxc.network.link = " + interface[0] + "\n")
    // file.write("lxc.network.ipv4 = " + interface[2] + " " + inf + "\n")
    // file.write("lxc.network.hwaddr = " + HWADDR_PREFIX + ":xx:xx:xx\n")
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
        //container exists, destroy
        if(!c->shutdown(c, 30)){
            //didn't shutdown
            c->stop(c);
        }
        c->destroy(c);
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
    if (!c->start(c, 0, NULL)) {
        //failed to start
        cerr << "Container " << nodePtr->name << " failed to start" << endl;
    }
}

void LxcContainerType::prepForInstall(std::vector<std::shared_ptr<ns3lxc::Application> > appList){

}

static void installThread(std::shared_ptr<ns3lxc::Node> nodePtr, std::string packman, std::string installCmd){
    string nullRedir = " 2>&1 > /dev/null";
    string lxcDir = "/var/lib/lxc/" + nodePtr->name + "/rootfs";
    //nav to root of container
    chdir(lxcDir.c_str());
    //chroot
    chroot(lxcDir.c_str());
    //install package based on local distro
    for(auto app : nodePtr->applications){
        //FIXME doesnt work
        execl(("usr/bin/" + packman).c_str(), (installCmd + app.name).c_str());
    }
}

void LxcContainerType::installApplications(std::shared_ptr<ns3lxc::Node> nodePtr) {
    string packman = distroPackMap.at(containerDistro);
    string installCmd = packmanMap.at(packman).at("install") + " ";
    pid_t parent = getpid();
    pid_t pid = fork();

    if (pid == -1)
    {
        // error, failed to fork()
    } 
    else if (pid > 0)
    {
        int status;
        waitpid(pid, &status, 0);
    }
    else 
    {
        // we are the child
        // execve(...);
        // _exit(EXIT_FAILURE);   // exec never returns
        installThread(nodePtr, packman, installCmd);
        exit(0);
    }
}
void LxcContainerType::runApplications(std::shared_ptr<ns3lxc::Node> nodePtr) {

}
void LxcContainerType::grabOutput(std::shared_ptr<ns3lxc::Node> nodePtr) {

}

void LxcContainerType::teardownContainer(std::shared_ptr<ns3lxc::Node> nodePtr){
    lxc_container *c = containerMap[nodePtr->name].get();
    if(c->is_defined(c)){
        //container exists, destroy
        if(!c->shutdown(c, 30)){
            //didn't shutdown
            c->stop(c);
        }
        c->destroy(c);
    }
    c = nullptr;
    containerMap.erase(nodePtr->name);

}