#ifndef __IFACE_H_INCLUDED__
#define __IFACE_H_INCLUDED__

// forward declared dependencies

// include dependencies
#include <string>
#include <memory>
#include <cstdint>

#include "ipaddr.h"
#include "nameable.h"

namespace ns3lxc {
class Node;
class Link;
// declarations
class Iface : public Nameable {

public:
    ns3lxc::Node *node = nullptr;
    ns3lxc::Link *link = nullptr;
    std::string bridgeName;
    std::string tapName;

    IpAddr *ip = nullptr;
    IpAddr *subnetMask = nullptr;

    std::array<uint8_t, 6> macAddr;

    Iface() {};
    Iface(const Iface&);
    Iface(std::string name): Nameable(name) {};
    Iface(std::string name, ns3lxc::Node *node): Nameable(name), node(node) {};
    ~Iface();

    void assignIp(int af, std::string ipAddr);
    void assignSubnetMask(int af, std::string subnetMaskStr);
    void assignSubnetMask(int af, int cidr);
};

class IfaceProvider {
public:
    std::map<std::string, std::string> ifacesProvidedSubNames; //maps this level's iface names to lower level's
    std::map<std::string, std::weak_ptr<ns3lxc::IfaceProvider> > ifacesProvided; //keep a ref to the providers we contain
    virtual ns3lxc::Iface *getIface(std::string ifaceName);
};

class IfaceAcceptor {
public:
    std::map<std::string, std::string> ifacesAcceptedSubNames; //maps this level's iface names to lower level's
    std::map<std::string, std::weak_ptr<ns3lxc::IfaceAcceptor> > ifacesAccepted; //keep a ref to who below us accepts ifaces
    virtual int connectIface(std::string ifaceName, Iface *iface);
};

}

#endif