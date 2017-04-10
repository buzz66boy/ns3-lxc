#ifndef __POSITION_H_INCLUDED__
#define __POSITION_H_INCLUDED__

// forward declared dependencies

// include dependencies
#include <vector>
#include "nameable.h"

namespace ns3lxc {

// declarations
class Position {
public:
    int x, y, z;
    int time;
};

class Positionable {
public:
    virtual ~Positionable () {}
	std::vector<Position> positions;
};

}
#endif