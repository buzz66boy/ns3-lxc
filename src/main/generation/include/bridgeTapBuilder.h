#ifndef __BRIDGE_TAP_BUILDER_H_INCLUDED__
#define __BRIDGE_TAP_BUILDER_H_INCLUDED__

#include <map>
#include <string>
#include <vector>
#include <memory>

#include "node.h"
#include "iface.h"

void buildAllBridgesTaps(std::vector<std::shared_ptr<ns3lxc::Node> >);

void buildBridgeTap(std::shared_ptr<ns3lxc::Iface>);

void tearDownAllBridgesTaps(std::vector<std::shared_ptr<ns3lxc::Node> > nodeList)

void tearDownBridgeTap(std::shared_ptr<ns3lxc::Iface>);

#endif