#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <utility>

#include "quagga.h"

using namespace std;

static std::string writeDaemonFile(shared_ptr<yntdl::Node> nodePtr);
static std::string writeConfigFile(shared_ptr<yntdl::Node> nodePtr);

std::vector<std::pair<std::string, std::string> > Quagga::getExistingRequiredFiles(yntdl::Application *app, std::shared_ptr<yntdl::Node> nodePtr){
    vector<pair<string, string> > existingFiles;
    return existingFiles;
}
std::vector<std::pair<std::string, std::string> > Quagga::getRequiredFiles(yntdl::Application *app, std::shared_ptr<yntdl::Node> nodePtr){
    vector<pair<string, string> > requiredFiles;
    requiredFiles.push_back(pair<string, string>("/etc/quagga/Quagga.conf", writeConfigFile(nodePtr)));
    requiredFiles.push_back(pair<string, string>("/etc/quagga/daemons", writeDaemonFile(nodePtr)));
    return requiredFiles;
}
    
std::vector<std::pair<std::string, bool> > Quagga::getExecutionCommands(yntdl::Application *app, std::shared_ptr<yntdl::Node>){
    vector<pair<string, bool> > commands;
    commands.push_back(pair<string, bool>("echo \"1\" > /proc/sys/net/ipv4/ip_forward", true));
    commands.push_back(pair<string, bool>("echo \"net.ipv4.ip_forward = 1\" >> /etc/sysctl.conf", true));
    commands.push_back(pair<string, bool>("/etc/init.d/quagga stop > /dev/null", true));
    commands.push_back(pair<string, bool>("/etc/init.d/quagga start > /dev/null", true));
    // if(args != ""){
    //     commands.push_back("/etc/init.d/quagga " + args + " > /dev/null");
    // }
    return commands;
}
std::vector<std::string> Quagga::getCleanupLocations(yntdl::Application *app, std::shared_ptr<yntdl::Node>){
    return vector<string>({"/etc/quagga/Quagga.conf", "/etc/quagga/daemons"});
}
std::vector<std::string> Quagga::getCleanupCommands(yntdl::Application *app, std::shared_ptr<yntdl::Node>){
    return vector<string>();
}

static std::string writeDaemonFile(shared_ptr<yntdl::Node> nodePtr){
    stringstream sstr;
    //Enable ospf (and zebra)
    sstr << \
R"(
zebra=yes
ospfd=yes
ospf6d=no
babeld=no
isisd=no
bgpd=no
ripd=no
ripngd=no
)" << endl;
    return sstr.str();
}

static std::string writeConfigFile(shared_ptr<yntdl::Node> nodePtr){
    stringstream sstr;
    sstr << "hostname " << nodePtr->name << "_quagga" << endl;
    sstr << "log stdout" << endl;
    sstr << "password zebra" << endl;
    sstr << "enable password zebra" << endl;

    //Link detect (and set cost if needed) for each iface
    for(auto ifacePair : nodePtr->ifaces){
        if(ifacePair.second.ip && ifacePair.second.subnetMask){
            sstr << "interface " << ifacePair.first << endl;
            // sstr << "\tip ospf cost " + cost << endl;
            sstr << "\tlink-detect" << endl;
        }
    }

    //Handle loopback
    sstr << "interface lo" << endl;
    sstr << "\tlink-detect" << endl;


    sstr << "ip forwarding" << endl;

    //configure ospf
    sstr << "router ospf" << endl;
    //sstr << "redistribute kernel" << endl; //Share kernel added routes
    sstr << "default-information originate" << endl;

    //add areas for each network attached to (this is where we'd exclude an iface)
    for(auto ifacePair : nodePtr->ifaces){
        if(ifacePair.second.ip && ifacePair.second.subnetMask){
            string subnet = ifacePair.second.ip->getSubnet(ifacePair.second.subnetMask);
            sstr << "network " + subnet + "/" + to_string(ifacePair.second.subnetMask->getCidr()) + " area 0" << endl;
        }
    }
    return sstr.str();
}