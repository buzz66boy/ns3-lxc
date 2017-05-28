#include <string>
#include <memory>
#include <iostream>

#include "settingsParser.h"
#include "nodeTypeMap.h"
#include "nodeSpawner.h"

using namespace std;

void NodeSpawner::createNodes(ns3lxc::Topology *top){
    for(auto subTopPtr : top->subTopologies){
        createNodes(subTopPtr.get());
    }
    for(shared_ptr<ns3lxc::Node> nodePtr : top->nodes){
        cout << "Creating Container " << nodePtr->name << endl;
        nodeTypeMap.at(nodePtr->type)->createNode(nodePtr);
    }
}
void NodeSpawner::startNodes(ns3lxc::Topology *top){
    for(auto subTopPtr : top->subTopologies){
        startNodes(subTopPtr.get());
    }
    for(shared_ptr<ns3lxc::Node> nodePtr : top->nodes){
        nodeTypeMap.at(nodePtr->type)->startNode(nodePtr);
    }
}
void NodeSpawner::installApplications(ns3lxc::Topology *top){
    for(auto subTopPtr : top->subTopologies){
        installApplications(subTopPtr.get());
    }
    for(shared_ptr<ns3lxc::Node> nodePtr : top->nodes){
        nodeTypeMap.at(nodePtr->type)->installApplications(nodePtr);
    }
}
void NodeSpawner::runApplications(ns3lxc::Topology *top){
    for(auto subTopPtr : top->subTopologies){
        runApplications(subTopPtr.get());
    }
    for(shared_ptr<ns3lxc::Node> nodePtr : top->nodes){
        nodeTypeMap.at(nodePtr->type)->runApplications(nodePtr);
    }
}
void NodeSpawner::grabOutput(ns3lxc::Topology *top){
    for(auto subTopPtr : top->subTopologies){
        grabOutput(subTopPtr.get());
    }
    for(shared_ptr<ns3lxc::Node> nodePtr : top->nodes){
        nodeTypeMap.at(nodePtr->type)->grabOutput(nodePtr);
    }
}

void NodeSpawner::teardownNodes(ns3lxc::Topology *top){
    for(auto subTopPtr : top->subTopologies){
        teardownNodes(subTopPtr.get());
    }
    for(shared_ptr<ns3lxc::Node> nodePtr : top->nodes){
        nodeTypeMap.at(nodePtr->type)->teardownNode(nodePtr);
    }
}