#ifndef __POSITION_H_INCLUDED__
#define __POSITION_H_INCLUDED__

// forward declared dependencies

// include dependencies
#include <vector>

namespace ns3lxc {

// declarations
class Positionable {
public:
	std::vector<Position> positions;
};

class Position {
public:
	int x, y, z;
	int time;
};

}
#endif