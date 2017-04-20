#ifndef __WIFI_H_INCLUDED__
#define __WIFI_H_INCLUDED__

#include <map>
#include <string>
#include <ostream>

#include "linkType.h"

class Wifi : public LinkType {
public:
    int getIfacesSupported() override;
    void writeIncludes(std::ostream str) override;
    void writeTypeInit(std::ostream str) override;
    void writeLinkInit(std::ostream str, std::string linkName, int linkNum) override;
    void addIfaceToLink(std::ostream str, std::string ifaceName, std::string linkName, int linkNum) override;
};

#endif