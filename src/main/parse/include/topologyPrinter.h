#ifndef __TOPOLOGY_PRINTER_H_INCLUDED__
#define __TOPOLOGY_PRINTER_H_INCLUDED__

#include "logger.h"

void printTopology(Logger, ParsedTopology);
void printTopology(Logger, ns3lxc::Topology);
void printNode(Logger, ns3lxc::Node);
void printLink(Logger, ns3lxc::Link);
void printApplication(Logger, ns3lxc::Application);

#endif
