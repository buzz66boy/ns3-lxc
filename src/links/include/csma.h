#ifndef __CSMA_H_INCLUDED__
#define __CSMA_H_INCLUDED__

#include <map>
#include <string>
#include <ostream>

#include "linkType.h"

class CSMA : public LinkType {
public:
    int getIfacesSupported();
    void writeIncludes(std::ostream str);
    void writeTypeInit(std::ostream str);
    void writeLinkInit(std::ostream str, std::string linkName, int linkNum);
    void addIfaceToLink(std::ostream str, std::string ifaceName, std::string linkName, int linkNum);
};

#endif