#include <string>
#include <memory>
#include <iostream>

#include "yntdl.h"
#include "nodeTypeMap.h"
#include "nodeSpawner.h"

using namespace std;

void NodeSpawner::createNodes(yntdl::Topology *top){
    for(auto subTopPtr : top->subTopologies){
        createNodes(subTopPtr.get());
    }
    for(shared_ptr<yntdl::Node> nodePtr : top->nodes){
        cout << "Creating Container " << nodePtr->name << endl;
        nodeTypeMap.at(nodePtr->type)->createNode(nodePtr);
    }
}
void NodeSpawner::startNodes(yntdl::Topology *top){
    for(auto subTopPtr : top->subTopologies){
        startNodes(subTopPtr.get());
    }
    for(shared_ptr<yntdl::Node> nodePtr : top->nodes){
        nodeTypeMap.at(nodePtr->type)->startNode(nodePtr);
    }
}
void NodeSpawner::installApplications(yntdl::Topology *top){
    for(auto subTopPtr : top->subTopologies){
        installApplications(subTopPtr.get());
    }
    for(shared_ptr<yntdl::Node> nodePtr : top->nodes){
        nodeTypeMap.at(nodePtr->type)->installApplications(nodePtr);
    }
}

void NodeSpawner::runCommands(yntdl::Topology *top){
    for(auto subTopPtr : top->subTopologies){
        runCommands(subTopPtr.get());
    }
    for(shared_ptr<yntdl::Node> nodePtr : top->nodes){
        nodeTypeMap.at(nodePtr->type)->runCommands(nodePtr);
    }
}

void NodeSpawner::runApplications(yntdl::Topology *top){
    for(auto subTopPtr : top->subTopologies){
        runApplications(subTopPtr.get());
    }
    for(shared_ptr<yntdl::Node> nodePtr : top->nodes){
        nodeTypeMap.at(nodePtr->type)->runApplications(nodePtr);
    }
}
void NodeSpawner::grabOutput(yntdl::Topology *top){
    for(auto subTopPtr : top->subTopologies){
        grabOutput(subTopPtr.get());
    }
    for(shared_ptr<yntdl::Node> nodePtr : top->nodes){
        nodeTypeMap.at(nodePtr->type)->grabOutput(nodePtr);
    }
}

void NodeSpawner::teardownNodes(yntdl::Topology *top){
    for(auto subTopPtr : top->subTopologies){
        teardownNodes(subTopPtr.get());
    }
    for(shared_ptr<yntdl::Node> nodePtr : top->nodes){
        nodeTypeMap.at(nodePtr->type)->teardownNode(nodePtr);
    }
}