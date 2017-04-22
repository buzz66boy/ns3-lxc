#include <vector>
#include <memory>
#include <string>
#include <cstdlib>
#include <iostream>

#include "node.h"
#include "iface.h"
#include "bridgeTapBuilder.h"

using namespace std;

void buildAllBridgesTaps(std::vector<std::shared_ptr<ns3lxc::Node> > nodeList){
    for(auto nodePtr : nodeList){
        for(auto it : nodePtr->ifaces){
            cout << nodePtr->name << ": " << it.first << endl;
            buildBridgeTap(it.second);
        }
    }
}

void buildBridgeTap(std::shared_ptr<ns3lxc::Iface> ifacePtr){
    int err;
    string tap = ifacePtr->tapName;
    string bridge = ifacePtr->bridgeName;
    cout << ifacePtr->name << " " << ifacePtr->ip << endl;
    //brctl addbr 'bridge'
    err = system(("brctl addbr " + bridge).c_str());
    if(err){

    }
    //ip tuntap add 'tap' mode tap
    err = system(("ip tuntap add " + tap + " mode tap").c_str());
    if(err){

    }
    //ifconfig 'tap' 0.0.0.0 promisc up
    err = system(("ifconfig " + tap + " 0.0.0.0 promisc up").c_str());
    if(err){

    }
    //brctl addif 'bridge' 'tap'
    err = system(("brctl addif " + bridge + " " + tap).c_str());
    if(err){

    }
    //ifconfig 'bridge' 'ipAddr' netmask 'subnetAddr'  ### adds two to IPADDR??? 
    err = system(("ifconfig " + bridge + " " + ifacePtr->ip->str() + \
        " netmask " + ifacePtr->subnetMask->str()).c_str());
    if(err){

    }
}

void tearDownAllBridgesTaps(std::vector<std::shared_ptr<ns3lxc::Node> > nodeList){
    for(auto nodePtr : nodeList){
        for(auto it : nodePtr->ifaces){
            tearDownBridgeTap(it.second);
        }
    }
}

void tearDownBridgeTap(std::shared_ptr<ns3lxc::Iface> ifacePtr){
    int err;
    string tap = ifacePtr->tapName;
    string bridge = ifacePtr->bridgeName;
    //ifconfig 'bridge' down
    err = system(("ifconfig " + bridge + " down").c_str());
    if(err){

    }
    //brctl delif 'bridge' 'tap'
    err = system(("brctl delif " + bridge + " " + tap).c_str());
    if(err){

    }
    //brctl delbr 'bridge'
    err = system(("brctl delbr " + bridge).c_str());
    if(err){

    }
    //ifconfig 'tap' down
    err = system(("ifconfig " + tap + " down").c_str());
    if(err){

    }
    //ip tuntap del 'tap' mode tap
    err = system(("ip tuntap del " + tap + " mode tap").c_str());
    if(err){

    }
}