#include <iostream>
#include <fstream>

#include <sys/param.h>
#include <unistd.h>

#include "settingsParser.h"
#include "nodeSpawner.h"
#include "bridgeTapBuilder.h"
#include "ns3Writer.h"
#include "linkTypeMap.h"

#include "topologyGenerator.h"

using namespace std;

void generateTopology(yntdl::Topology *topology){
    assignBridgesTaps(topology);
    findLinkTypesUsed(topology);
    if(Settings::genContainers()){
        tearDownAllBridgesTaps(topology);
        buildAllBridgesTaps(topology);
        NodeSpawner::createNodes(topology);
        NodeSpawner::startNodes(topology);
        NodeSpawner::installApplications(topology);
    }

    string scriptLoc = Settings::script_dest + "/" + topology->name + ".cc";
    if(Settings::genNS3()){
        Ns3Writer::writeScript(topology, scriptLoc);
    }
    if(Settings::runNS3()){
        cout << "Waiting to continue (enter): ";
        cout.flush();
        cin.ignore();
        cout << "Continuing..." << endl;
        std::ifstream src(scriptLoc, std::ios::binary);
        std::ofstream dst(Settings::ns3_path + "/scratch/" + topology->name + ".cc",   std::ios::binary);
        dst << src.rdbuf();
        src.close();
        dst.close();
        char cwd[PATH_MAX];
        getcwd(cwd,  PATH_MAX + 1);
        chdir(Settings::ns3_path.c_str());
        system("./waf"); //Makes more consistent run when everything's build first
        NodeSpawner::runCommands(topology);
        NodeSpawner::runApplications(topology);
        if(Settings::gdbNS3()){
            system(("./waf --run scratch/" + topology->name + " --command-template=\"gdb --args %s\"").c_str());
        } else {
            system(("./waf --run scratch/" + topology->name).c_str());
        }
        cout << endl;
        Ns3Writer::grabOutput(string(cwd) + '/' + Settings::top_output_dest);
        chdir(cwd);
        NodeSpawner::grabOutput(topology);
    }
    if(Settings::teardown()){
        cout << "Tearing Down Topology" << endl;
        NodeSpawner::teardownNodes(topology);
        tearDownAllBridgesTaps(topology);
    }
}

void findLinkTypesUsed(yntdl::Topology *topology){
    for(auto linkPtr : topology->links){
        linkTypeMap.find(linkPtr->getType())->second->setUsed();
    }
    for(auto subTopPtr : topology->subTopologies){
        findLinkTypesUsed(subTopPtr.get());
    }
}