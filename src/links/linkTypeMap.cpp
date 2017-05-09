#include "linkTypeMap.h"

 const std::map<std::string, LinkType*> linkTypeMap = {{"csma", new CSMA()}, {"wifi", new Wifi()}};