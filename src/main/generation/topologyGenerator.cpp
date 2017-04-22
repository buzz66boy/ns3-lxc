#include <iostream>

#include "nodeSpawner.h"
#include "bridgeTapBuilder.h"
#include "ns3Writer.h"

#include "topologyGenerator.h"

using namespace std;

void generateTopology(ns3lxc::Topology *topology){
    GeneratedTopology genTop(topology);
    tearDownAllBridgesTaps(topology->nodes);
    buildAllBridgesTaps(topology->nodes);
    NodeSpawner::createNodes(topology);
    NodeSpawner::startNodes(topology);

    cout << "Waiting to continue (enter): ";
    cout.flush();
    string test;
    cin >> test;
    cout << "Continuing..." << endl;
    NodeSpawner::teardownNodes(topology);
    tearDownAllBridgesTaps(topology->nodes);

}