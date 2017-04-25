#include <iostream>
#include <fstream>

#include <sys/param.h>
#include <unistd.h>

#include "settingsParser.h"
#include "nodeSpawner.h"
#include "bridgeTapBuilder.h"
#include "ns3Writer.h"

#include "topologyGenerator.h"

using namespace std;

void generateTopology(ns3lxc::Topology *topology){
    GeneratedTopology genTop(topology);
    assignBridgesTaps(topology);

    tearDownAllBridgesTaps(topology);
    buildAllBridgesTaps(topology);
    NodeSpawner::createNodes(topology);
    NodeSpawner::startNodes(topology);

    string scriptLoc = Settings::script_dest + "/" + topology->name + ".cc";
    Ns3Writer::writeScript(topology, scriptLoc);
    cout << "Waiting to continue (enter): ";
    cout.flush();
    string test;
    cin >> test;
    cout << "Continuing..." << endl;
    std::ifstream src(scriptLoc, std::ios::binary);
    std::ofstream dst(Settings::ns3_path + "/scratch/" + topology->name + ".cc",   std::ios::binary);
    dst << src.rdbuf();
    src.close();
    dst.close();

    chdir(Settings::ns3_path.c_str());
    system(("./waf --run scratch/" + topology->name).c_str());
    NodeSpawner::teardownNodes(topology);
    tearDownAllBridgesTaps(topology);

}