#ifndef __LINK_TYPE_H_INCLUDED__
#define __LINK_TYPE_H_INCLUDED__

#include <map>
#include <string>
#include <ostream>

class LinkType {
    virtual ~LinkType() = 0;
public:
    int getIfacesSupported();
    void writeTypeInit(std::ostream str);
    void writeLinkInit(std::ostream str, std::string linkName, int linkNum);
    void addIfaceToLink(std::ostream str, std::string ifaceName, std::string linkName, int linkNum);

};

#endif