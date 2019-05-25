#ifndef __TOPOLOGY_PRINTER_H_INCLUDED__
#define __TOPOLOGY_PRINTER_H_INCLUDED__

#include "logger.h"

void printTopology(Logger, ParsedTopology);
void printTopology(Logger, yntdl::Topology);
void printNode(Logger, yntdl::Node);
void printLink(Logger, yntdl::Link);
void printApplication(Logger, yntdl::Application);

#endif
