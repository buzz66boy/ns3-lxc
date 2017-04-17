#include <string>

#include <lxc/lxccontainer.h>

#include "lxcSpawner.h"

using namespace std;

const string LXCSpawner::defaultContainer = "ubuntu";

void LXCSpawner::createContainers(ns3lxc::Topology *){
    lxc_container *c;
}
void LXCSpawner::startContainers(ns3lxc::Topology *){
}
void LXCSpawner::installApplications(ns3lxc::Topology *){

}
void LXCSpawner::runApplications(ns3lxc::Topology *){
}
void LXCSpawner::grabOutput(ns3lxc::Topology *){

}