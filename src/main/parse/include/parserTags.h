#ifndef __PARSER_TAGS_H_INCLUDED__
#define __PARSER_TAGS_H_INCLUDED__

#include <vector>
#include <string>

// NOTE: these tags can also be used plurally (checks for tag + "s")
#define TAG_INCLUDE "include" 
#define TAG_TIME "time"
#define TAG_NODE "node" 
#define TAG_TOPOLOGY "topology"
#define TAG_APPLICATION "application"
#define TAG_LINK "link"
#define TAG_POSITION "position"
#define TAG_IFACE "iface"
#define TAG_INTERFACE "interface"
#define TAG_TEMPLATE "template"
#define TAG_NUM "num"
#define TAG_ALL "all"
#define TAG_INHERIT "inherit"
#define TAG_ROTATION "rotation"
#define TAG_ACCEPT_IFACE "acceptIface"
#define TAG_IFACES_ACCEPTED "ifacesAccepted"
#define TAG_IFACES_PROVIDED "ifacesProvided"
#define TAG_NAME "name"
#define TAG_IP "ip"
#define TAG_CIDR "cidr"
#define TAG_SUBNET "subnet"
#define TAG_TYPE "type"
#define TAG_OFFSET "offset"

//Utility functions for dealing with tags and topologies
std::string pluralize(std::string str);
std::vector<std::string> splitString(std::string str);

#endif