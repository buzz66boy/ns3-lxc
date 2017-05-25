#include <string>
#include <vector>
#include <map>
#include <memory>
#include <iostream>

#include "ipaddr.h"
#include "nameable.h"
#include "iface.h"

using namespace ns3lxc;

Iface::Iface(const Iface& ifa): Nameable(ifa) {
    node = ifa.node;
    link = ifa.link;
    if(ifa.ip){
        ip = new IpAddr(*ifa.ip);
    }
    if(ifa.subnetMask){
        subnetMask = new IpAddr(*ifa.subnetMask);
    }
    tapName = ifa.tapName;
    bridgeName = ifa.bridgeName;
    macAddr = ifa.macAddr;
}

Iface::~Iface(){
    if(ip){
        delete ip;
    }
    if(subnetMask){
        delete subnetMask;
    }
}

void Iface::assignIp(int af, std::string ipAddr){
    if(ip){
        delete ip;
    }
    ip = new IpAddr(af, ipAddr);
}

void Iface::assignSubnetMask(int af, std::string subnetMaskStr){
    if(subnetMask){
        delete subnetMask;
    }
    subnetMask = new IpAddr(af, subnetMaskStr);
}

void Iface::assignSubnetMask(int af, int cidr){
    if(subnetMask){
        delete subnetMask;
    }
    subnetMask = new IpAddr(af, cidr);
}

ns3lxc::Iface *ns3lxc::IfaceProvider::getIface(std::string ifaceName){
    if(ifacesProvided.count(ifaceName) > 0){
        return ifacesProvided[ifaceName].lock()->getIface( ifacesProvidedSubNames[ifaceName] );
    }
    return nullptr;
}

int ns3lxc::IfaceAcceptor::connectIface(std::string ifaceName, ns3lxc::Iface *iface){
    if(ifacesAccepted.count(ifaceName) > 0){
        return ifacesAccepted[ifaceName].lock()->connectIface( ifacesAcceptedSubNames[ifaceName], iface );
    }
    return 0;
}