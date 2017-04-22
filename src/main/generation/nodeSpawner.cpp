#include <string>
#include <memory>
#include <iostream>

#include <lxc/lxccontainer.h>

#include "settingsParser.h"
#include "containerTypeMap.h"
#include "nodeSpawner.h"

using namespace std;

void NodeSpawner::createNodes(ns3lxc::Topology *top){
    for(shared_ptr<ns3lxc::Node> nodePtr : top->nodes){
        cout << "Creating Container " << nodePtr->name << endl;
        containerTypeMap[Settings::container_type]->createContainer(nodePtr);
    }
}
void NodeSpawner::startNodes(ns3lxc::Topology *top){
    for(shared_ptr<ns3lxc::Node> nodePtr : top->nodes){
        containerTypeMap[Settings::container_type]->startContainer(nodePtr);
    }
}
void NodeSpawner::installApplications(ns3lxc::Topology *){

}
void NodeSpawner::runApplications(ns3lxc::Topology *){
}
void NodeSpawner::grabOutput(ns3lxc::Topology *){

}

void NodeSpawner::teardownNodes(ns3lxc::Topology *top){
    for(shared_ptr<ns3lxc::Node> nodePtr : top->nodes){
        containerTypeMap[Settings::container_type]->teardownContainer(nodePtr);
    }
}