#ifndef __BRIDGE_TAP_BUILDER_H_INCLUDED__
#define __BRIDGE_TAP_BUILDER_H_INCLUDED__

#include <map>
#include <string>
#include <vector>
#include <memory>

#include "node.h"
#include "iface.h"
#include "topology.h"

void assignBridgesTaps(ns3lxc::Topology *top);

void buildAllBridgesTaps(ns3lxc::Topology *top);

void buildBridgeTap(std::shared_ptr<ns3lxc::Iface>);

void tearDownAllBridgesTaps(ns3lxc::Topology *top);

void tearDownBridgeTap(std::shared_ptr<ns3lxc::Iface>);

#endif