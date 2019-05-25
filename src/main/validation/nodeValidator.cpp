#include <memory>
#include <iostream>
#include <string>

#include "node.h"
#include "link.h"
#include "iface.h"
#include "errorCode.h"
#include "nodeValidator.h"

using namespace std;

void validateNode(std::shared_ptr<yntdl::Node> nodePtr){
    bool fail = false;
    if(nodePtr->name == ""){
        cerr << "Node does not have a name" << endl;
        fail = true;
    }
    string name = " " + nodePtr->name + " ";
    if(nodePtr->type == ""){
        cerr << "Node" + name + "does not have a type" << endl;
        fail = true;
    }
    if(nodePtr->ifaces.size() == 0){
        cerr << "WARNING: Node" + name + "was not assigned an interface" << endl;
    }
    if(nodePtr->nodeNum == -1){
        cerr << "Node" + name + " was not assigned a node number" << endl;
        fail = true;
    }

    for(auto ifacePair : nodePtr->ifaces){
        yntdl::Iface iface = ifacePair.second;
        if(iface.name == ""){
            cerr << "Iface on" + name + "was not assigned a name" << endl;
            fail = true;
        }
        string ifaceStr = "Iface " + iface.name + " on Node" + name;
        if(iface.macAddr == ""){
            cerr << "WARNING: " + ifaceStr + "not assigned a mac address" << endl;
        }
        if(iface.node != nodePtr.get()){
            cerr << ifaceStr + "does not point back to same node" << endl;
            fail = true;
        }
        if(iface.link == nullptr){
            cout << "INFO: " + ifaceStr + "not connected to a link" << endl;
        }
    }

    for(auto app : nodePtr->applications){
        if(app.name == ""){
            cerr << "Application on" + name + "has no name" << endl;
            throw Ns3lxcException(ErrorCode::APP_INVALID, name);
        }
    }

    if(fail){
        throw Ns3lxcException(ErrorCode::NODE_INVALID, name);
    }
}