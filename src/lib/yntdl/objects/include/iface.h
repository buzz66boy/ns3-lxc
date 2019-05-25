#ifndef __IFACE_H_INCLUDED__
#define __IFACE_H_INCLUDED__

// forward declared dependencies

// include dependencies
#include <string>
#include <memory>
#include <cstdint>

#include "ipaddr.h"
#include "nameable.h"

namespace yntdl {
class Node;
class Link;
// declarations
class Iface : public Nameable {

public:
    yntdl::Node *node = nullptr;
    yntdl::Link *link = nullptr;
    std::string bridgeName;
    std::string tapName;
    std::string macAddr;

    IpAddr *ip = nullptr;
    IpAddr *subnetMask = nullptr;

    Iface() {};
    Iface(const Iface&);
    Iface(std::string name): Nameable(name) {};
    Iface(std::string name, yntdl::Node *node): Nameable(name), node(node) {};
    ~Iface();

    void assignIp(IpAddr *ipAddr);
    void assignIp(int af, std::string ipAddr);

    void assignSubnetMask(IpAddr *subnetMaskAddr);
    void assignSubnetMask(int af, std::string subnetMaskStr);
    void assignSubnetMask(int af, int cidr);
};

class IfaceProvider {
protected:
    IfaceProvider() {};
public:
    std::map<std::string, std::string> ifacesProvidedSubNames; //maps this level's iface names to lower level's
    std::map<std::string, std::weak_ptr<yntdl::IfaceProvider> > ifacesProvided; //keep a ref to the providers we contain
    virtual yntdl::Iface *getIface(std::string ifaceName);
};

class IfaceAcceptor {
protected:
    IfaceAcceptor() {};
public:
    std::map<std::string, std::string> ifacesAcceptedSubNames; //maps this level's iface names to lower level's
    std::map<std::string, std::weak_ptr<yntdl::IfaceAcceptor> > ifacesAccepted; //keep a ref to who below us accepts ifaces
    virtual int connectIface(std::string ifaceName, Iface *iface);
};

}

#endif