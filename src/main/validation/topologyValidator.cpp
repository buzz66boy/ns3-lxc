#include <string>
#include <vector>
#include <iostream>

#include "errorCode.h"
#include "yntdl.h"
#include "linkTypeMap.h"
#include "nodeTypeMap.h"
#include "topologyValidator.h"

using namespace std;

void validateNodeType(std::shared_ptr<yntdl::Node> nodePtr){
    bool fail = false;
    //Check that node type exists in nodeTypeMap
    if(nodeTypeMap.find(nodePtr->type) == nodeTypeMap.end()){
        cerr << "Node " + nodePtr->name + " has invalid type " + nodePtr->type << endl;
        fail = true;
    }

    if(fail){
        throw Ns3lxcException(ErrorCode::NODE_INVALID, nodePtr->name);
    }
}

void validateLinkType(std::shared_ptr<yntdl::Link> linkPtr){
    bool fail = false;
    string name = " " + linkPtr->name + " ";
    //Check that link type exists in linkTypeMap
    if(linkTypeMap.find(linkPtr->getType()) == linkTypeMap.end()){
        cerr << "Link" + name + "has invalid type " + linkPtr->getType() << endl;
        fail = true;
    } 
    //Check that link has a correct number of interfaces connected for the type
    else if(!fail && linkPtr->ifaces.size() > linkTypeMap.at(linkPtr->getType())->getIfacesSupported()){ 
        cerr << "Link" + name + "has more interfaces connected than the Link Type supports" << endl;
        fail = true;
    }

    if(fail){
        throw Ns3lxcException(ErrorCode::LINK_INVALID, name);
    }
}

void validateTopologyTypes(yntdl::Topology *top){
    bool fail = false;

    for(auto nodePtr : top->nodes){
        validateNodeType(nodePtr);
    }
    for(auto linkPtr : top->links){
        validateLinkType(linkPtr);
    }
    for(auto subTopPtr : top->subTopologies){
        validateTopologyTypes(subTopPtr.get());
    }

    if(fail){
        throw Ns3lxcException(ErrorCode::TOPOLOGY_INVALID, top->name);
    }
}