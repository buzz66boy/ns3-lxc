#include <iostream>

#include "nodeSpawner.h"
#include "bridgeTapBuilder.h"
#include "ns3Writer.h"

#include "topologyGenerator.h"

using namespace std;

void generateTopology(ns3lxc::Topology *topology){
    GeneratedTopology genTop(topology);
    NodeSpawner::createNodes(topology);
    NodeSpawner::startNodes(topology);
    NodeSpawner::teardownNodes(topology);

}