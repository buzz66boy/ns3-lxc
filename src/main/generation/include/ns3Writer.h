#ifndef __NS3WRITER_H_INCLUDED__
#define __NS3WRITER_H_INCLUDED__

#include <map>
#include <string>
#include <ostream>

#include "topology.h"

class Ns3Writer {
public:
    static void writeScript(ns3lxc::Topology *top, std::string scriptLoc);
    static void writeIncludes(std::ostream& str);
    static void writeTimingFunction(std::ostream& str);
    static void writeInit(std::ostream& str, ns3lxc::Topology *top);
    static void writeLinksForTopology(std::ostream& str, ns3lxc::Topology *top);
    static void writePositions(std::ostream& str, ns3lxc::Topology *top);
};

#endif