#ifndef __LINK_TYPE_H_INCLUDED__
#define __LINK_TYPE_H_INCLUDED__

#include <map>
#include <string>
#include <ostream>

class LinkType {
public:
    virtual int getIfacesSupported() = 0;
    virtual void writeIncludes(std::ostream str) = 0;
    virtual void writeTypeInit(std::ostream str) = 0;
    virtual void writeLinkInit(std::ostream str, std::string linkName, int linkNum) = 0;
    virtual void addIfaceToLink(std::ostream str, std::string ifaceName, std::string linkName, int linkNum) = 0;
};

#endif