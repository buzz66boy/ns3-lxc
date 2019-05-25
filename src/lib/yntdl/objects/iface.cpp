#include <string>
#include <vector>
#include <map>
#include <memory>
#include <iostream>

#include "ipaddr.h"
#include "nameable.h"
#include "iface.h"

using namespace yntdl;

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

void Iface::assignIp(IpAddr *ipAddr){
    if(ip){
        delete ip;
    }
    ip = new IpAddr(*ipAddr);
}

void Iface::assignIp(int af, std::string ipAddr){
    if(ip){
        delete ip;
    }
    ip = new IpAddr(af, ipAddr);
}

void Iface::assignSubnetMask(IpAddr *subnetMaskAddr){
    if(subnetMask){
        delete subnetMask;
    }
    subnetMask = new IpAddr(*subnetMaskAddr);
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

yntdl::Iface *yntdl::IfaceProvider::getIface(std::string ifaceName){
    if(ifacesProvided.count(ifaceName) > 0){
        return ifacesProvided[ifaceName].lock()->getIface( ifacesProvidedSubNames[ifaceName] );
    }
    return nullptr;
}

int yntdl::IfaceAcceptor::connectIface(std::string ifaceName, yntdl::Iface *iface){
    if(ifacesAccepted.count(ifaceName) > 0){
        return ifacesAccepted[ifaceName].lock()->connectIface( ifacesAcceptedSubNames[ifaceName], iface );
    }
    return 0;
}