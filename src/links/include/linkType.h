#ifndef __LINK_TYPE_H_INCLUDED__
#define __LINK_TYPE_H_INCLUDED__

#include <map>
#include <string>
#include <ostream>
#include <memory>

#include "link.h"

class LinkType {
    static bool used;
public:
    void setUsed() {LinkType::used = true;}
    bool isUsed() {return LinkType::used;}

    virtual int getIfacesSupported() = 0;
    virtual void writeIncludes(std::ostream& str) = 0;
    virtual void writeTypeInit(std::ostream& str) = 0;
    virtual void writeLinkInit(std::ostream& str, std::shared_ptr<ns3lxc::Link> linkPtr) = 0;
    virtual void addIfacesToLink(std::ostream& str, std::shared_ptr<ns3lxc::Link> linkPtr) = 0;
};

#endif