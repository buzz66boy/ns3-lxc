#ifndef __IFACE_H_INCLUDED__
#define __IFACE_H_INCLUDED__

// forward declared dependencies

// include dependencies
#include <string>
#include <memory>
#include "ipaddr.h"
#include "nameable.h"

namespace ns3lxc {
class Node;
class Link;
// declarations
class Iface : public Nameable {
public:
    ns3lxc::Node *node;
    std::weak_ptr<ns3lxc::Link> link;
    IpAddr *address;

    Iface() {};
    Iface(const Iface&);
    Iface(std::string name): Nameable(name) {};
    Iface(std::string name, ns3lxc::Node *node): Nameable(name), node(node) {};
};

class IfaceProvider : virtual public Nameable {
public:
    virtual ~IfaceProvider () {}
    std::map<std::string, std::string> ifacesProvidedSubNames; //maps this level's iface names to lower level's
    std::map<std::string, std::weak_ptr<ns3lxc::IfaceProvider> > ifacesProvided; //keep a ref to the providers we contain
    std::weak_ptr<ns3lxc::Iface> getIface(std::string ifaceName);
};

class IfaceAccepter : virtual public Nameable {
public:
    virtual ~IfaceAccepter () {}
    std::map<std::string, std::string> ifacesAcceptedSubNames; //maps this level's iface names to lower level's
    std::map<std::string, std::weak_ptr<ns3lxc::IfaceAccepter> > ifacesAccepted; //keep a ref to who below us accepts ifaces
    int connectIface(std::string ifaceName, std::weak_ptr<Iface> iface);
};

}

#endif