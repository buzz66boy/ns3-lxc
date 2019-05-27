#ifndef __LINK_TYPE_H_INCLUDED__
#define __LINK_TYPE_H_INCLUDED__

#include <map>
#include <string>
#include <ostream>
#include <memory>

#include "yntdl.h"

class LinkType {
    bool used = false;
public:
    void setUsed() {used = true;}
    bool isUsed() {return used;}

    virtual int getIfacesSupported() = 0;
    virtual void writeIncludes(std::ostream& str) = 0;
    virtual void writeTypeInit(std::ostream& str) = 0;
    virtual void writeLinkInit(std::ostream& str, std::shared_ptr<yntdl::Link> linkPtr) = 0;
    virtual void addIfacesToLink(std::ostream& str, std::shared_ptr<yntdl::Link> linkPtr) = 0;
    virtual void writeClosingRemarks(std::ostream& str) = 0;
};

#endif