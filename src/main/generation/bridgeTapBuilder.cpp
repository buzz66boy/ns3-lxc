#include <vector>
#include <memory>
#include <string>
#include <cstdlib>

#include "node.h"
#include "iface.h"
#include "bridgeTapBuilder.h"

using namespace std;

void buildAllBridgesTaps(std::vector<std::shared_ptr<ns3lxc::Node> > nodeList){
    for(nodePtr : nodeList){
        for(it : nodePtr->ifaces){
            buildBridgeTap(it.second);
        }
    }
}

void buildBridgeTap(std::shared_ptr<ns3lxc::Iface> ifacePtr){
    int err;
    string tap = ifacePtr->tapName;
    string bridge = ifacePtr->bridgeName;
    //brctl addbr 'bridge'
    err = system("brctl addbr " + bridge);
    if(err){

    }
    //ip tuntap add 'tap' mode tap
    err = system("ip tuntap add " + tap + " mode tap");
    if(err){

    }
    //ifconfig 'tap' 0.0.0.0 promisc up
    err = system("ifconfig " + tap + " 0.0.0.0 promisc up");
    if(err){

    }
    //brctl addif 'bridge' 'tap'
    err = system("brctl addif " + bridge + " " + tap);
    if(err){

    }
    //ifconfig 'bridge' 'ipAddr' netmask 'subnetAddr'  ### adds two to IPADDR??? 
    err = system("ifconfig " + bridge + " " + ifacePtr->ip->str() + \
        " netmask " + ifacePtr->subnetMask->str());
    if(err){

    }
}

void tearDownAllBridgesTaps(std::vector<std::shared_ptr<ns3lxc::Node> > nodeList){
    for(nodePtr : nodeList){
        for(it : nodePtr->ifaces){
            tearDownBridgeTap(it.second);
        }
    }
}

void tearDownBridgeTap(std::shared_ptr<ns3lxc::Iface>){
    //ifconfig 'bridge' down
    //brctl delif 'bridge' 'tap'
    //brctl delbr 'bridge'
    //ifconfig 'tap' down
    //ip tuntap del 'tap' mode tap
}