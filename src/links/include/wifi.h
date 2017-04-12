#ifndef __WIFI_H_INCLUDED__
#define __WIFI_H_INCLUDED__

#include <map>
#include <string>
#include <ostream>

#include "linkType.h"

class WIFI : public LinkType {
public:
    int getIfacesSupported();
    void writeIncludes(std::ostream str);
    void writeTypeInit(std::ostream str);
    void writeLinkInit(std::ostream str, std::string linkName, int linkNum);
    void addIfaceToLink(std::ostream str, std::string ifaceName, std::string linkName, int linkNum);
};

#endif