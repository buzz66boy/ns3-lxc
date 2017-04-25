#include <vector>
#include <memory>
#include <string>
#include <cstdlib>
#include <iostream>

#include "node.h"
#include "iface.h"
#include "bridgeTapBuilder.h"

using namespace std;

void assignBridgesTaps(ns3lxc::Topology *top){
    for(auto nodePtr : top->nodes){
        int i = 0;
        for(auto it : nodePtr->ifaces){
            cout << nodePtr->name << ": " << it.first << endl;
            it.second->bridgeName = nodePtr->name + "_" + to_string(i) + "_b";
            it.second->tapName = nodePtr->name + "_" + to_string(i) + "_t";
            i++;
        }
    }
}

void buildAllBridgesTaps(ns3lxc::Topology *top){
    for(auto nodePtr : top->nodes){
        int i = 0;
        for(auto it : nodePtr->ifaces){
            
            if(it.second->ip != nullptr && it.second->subnetMask != nullptr){
                cout << nodePtr->name << ": " << it.first << endl;
                buildBridgeTap(it.second);
                i++;
            }
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
        cerr << "BAD" << endl;
    }
    //ip tuntap add 'tap' mode tap
    err = system(("ip tuntap add " + tap + " mode tap").c_str());
    if(err){
        cerr << "BAD2" << endl;
    }
    //ifconfig 'tap' 0.0.0.0 promisc up
    err = system(("ifconfig " + tap + " 0.0.0.0 promisc up").c_str());
    if(err){
        cerr << "BAD3" << endl;
    }
    //brctl addif 'bridge' 'tap'
    err = system(("brctl addif " + bridge + " " + tap).c_str());
    if(err){
        cerr << "BAD4" << endl;
    }

    //ifconfig 'bridge' 'ipAddr' netmask 'subnetAddr' up  ### adds two to IPADDR??? 
    string sub = ifacePtr->ip->str();
    sub = sub.substr(0, sub.find_last_of(".")) + '2';
    err = system(("ifconfig " + bridge + " " + sub + \
        " netmask " + ifacePtr->subnetMask->str() + " up").c_str());
    if(err){
        cerr << "BAD5" << endl;
    }
}

void tearDownAllBridgesTaps(ns3lxc::Topology *top){
    for(auto nodePtr : top->nodes){
        for(auto it : nodePtr->ifaces){
            if(it.second->ip != nullptr && it.second->subnetMask != nullptr){
                tearDownBridgeTap(it.second);
            }
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