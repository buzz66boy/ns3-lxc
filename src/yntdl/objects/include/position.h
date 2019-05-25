#ifndef __POSITION_H_INCLUDED__
#define __POSITION_H_INCLUDED__

// forward declared dependencies

// include dependencies
#include <vector>
#include <string>

#include "nameable.h"

namespace ns3lxc {

// declarations
class Position {
public:
    double x, y, z;
    double time;
    Position(double time, double x, double y, double z): time(time), x(x), y(y), z(z) {};
    Position() {}
    Position(const Position &p): Position(p.time, p.x, p.y, p.z) {};

    std::string str();
    std::string ns3Str();
};

class Positionable {
public:
    virtual ~Positionable () {}
    Positionable() {}
    Positionable(const Positionable&);
    Positionable *parent = nullptr;

	std::vector<Position> positions; // Relative positions
    std::vector<Position> absPositions; // Absolute positions

    /**
     * Computes relative positions based on its positions and parents movement
     */
    void centerPositionsAroundParent(Positionable *par = nullptr);
    /**
     * Rotates positions (and hence movement) around 0, 0, 0 counter-clockwise
     */
    void rotatePositions(double degrees);
    /**
     * Computes position of positionable at a given time
     */
    Position getAbsPosition(double time);
    Position getPosition(double time);
};

}
#endif