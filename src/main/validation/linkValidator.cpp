#include <iostream>
#include <memory>
#include <string>

#include "link.h"
#include "iface.h"
#include "errorCode.h"
#include "linkTypeMap.h"
#include "linkValidator.h"

using namespace std;

void validateLink(std::shared_ptr<ns3lxc::Link> linkPtr){
    bool fail = false;
    if(linkPtr->name == ""){
        cerr << "Link does not have a name" << endl;
        fail = true;
    }
    string name = " " + linkPtr->name + " ";
    if(linkPtr->getType() == ""){
        cerr << "Link" + name + "has no type" << endl;
        fail = true;
    }
    if(linkPtr->ifaces.size() < 1){
        cout << "INFO: Link" + name + "does not have any connected interfaces" << endl;
    }
    if(linkPtr->ifaces.size() > linkTypeMap.at(linkPtr->getType())->getIfacesSupported()){
        cerr << "Link" + name + "has more interfaces connected than the Link Type supports" << endl;
        fail = true;
    }
    for(auto ifacePtr : linkPtr->ifaces){
        string ifaceStr = "Iface " + ifacePtr->name + " connected to link" + name;
        if(ifacePtr->link != linkPtr.get()){
            cerr << ifaceStr + "does not point back to same link" << endl;
            fail = true;
        }
        if(ifacePtr->ip == nullptr){
            cerr << ifaceStr + "is not assigned an IP address" << endl;
            fail = true;
        }
        if(ifacePtr->subnetMask == nullptr){
            cerr << ifaceStr + "is not assigned a subnet mask" << endl;
            fail = true;
        } else if(!ifacePtr->subnetMask->equals(*linkPtr->subnetMask)){
            cerr << ifaceStr + "not assigned same subnet mask as link" << endl;
            fail = true;
        }
    }
    if(fail){
        throw Ns3lxcException(ErrorCode::LINK_INVALID, name);
    }
}