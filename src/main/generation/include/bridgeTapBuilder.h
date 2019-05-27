#ifndef __BRIDGE_TAP_BUILDER_H_INCLUDED__
#define __BRIDGE_TAP_BUILDER_H_INCLUDED__

#include <map>
#include <string>
#include <vector>
#include <memory>

#include "yntdl.h"

int assignBridgesTaps(yntdl::Topology *top, int curNum = 0);

void buildAllBridgesTaps(yntdl::Topology *top);

void buildBridgeTap(yntdl::Iface*);

void tearDownAllBridgesTaps(yntdl::Topology *top);

void tearDownBridgeTap(yntdl::Iface*);

#endif