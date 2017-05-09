#ifndef __LINK_TYPE_MAP_H_INCLUDED__
#define __LINK_TYPE_MAP_H_INCLUDED__

#include <map>
#include <string>

#include "linkType.h"
#include "csma.h"
#include "wifi.h"

extern const std::map<std::string, LinkType*> linkTypeMap;

#endif