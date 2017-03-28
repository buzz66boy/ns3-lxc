#ifndef __IFACE_H_INCLUDED__
#define __IFACE_H_INCLUDED__

// forward declared dependencies

// include dependencies
#include <string>
#include <memory>
#include "ipaddr.h"

namespace ns3lxc {
class Node;
class Link;
// declarations
class Iface {
public:
    std::string name;
    ns3lxc::Node *node;
    ns3lxc::Link *link;
    IpAddr *address;

    Iface() {};
    Iface(std::string name): name(name) {};
};

class IfaceProvider {
public:
    std::map<std::string, std::string> ifacesProvidedSubNames; //maps this level's iface names to lower level's
    std::map<std::string, std::shared_ptr<ns3lxc::IfaceProvider> > ifacesProvided; //keep a ref to the providers we contain
    std::shared_ptr<ns3lxc::Iface> getIface(std::string ifaceName);
};

class IfaceAccepter {
public:
    std::map<std::string, std::string> ifacesAcceptedSubNames; //maps this level's iface names to lower level's
    std::map<std::string, std::shared_ptr<ns3lxc::IfaceAccepter> > ifacesAccepted; //keep a ref to who below us accepts ifaces
    int connectIface(std::string ifaceName, std::shared_ptr<Iface> iface);
};

}

#endif