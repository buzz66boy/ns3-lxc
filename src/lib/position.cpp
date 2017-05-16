#include <cmath>
#include <iostream>

#include "position.h"

using namespace ns3lxc;

std::string Position::str(){
    return ("Time: " + std::to_string(time)
    + " x: " + std::to_string(x)
    + " y: " + std::to_string(y)
    + " z: " + std::to_string(z));
}

std::string Position::ns3Str(){
    return ("Seconds(" + std::to_string(time) + "), Vector("
    + std::to_string(x) + "," 
    + std::to_string(y) + ","
    + std::to_string(z) + ")");
}

Positionable::Positionable(const Positionable& temp){
    parent = temp.parent;
    positions = temp.positions;
    absPositions = temp.absPositions;
}

/**
 * Computes relative positions based on its positions and parents movement
 */
void Positionable::centerPositionsAroundParent(Positionable *par){
    parent = par;
    if(parent != nullptr && (parent->absPositions.size() > 0 || parent->positions.size() > 0)){
        absPositions.clear();
        std::vector<Position> *posit;
        if(parent->absPositions.size() > 0){
            posit = &parent->absPositions;
        } else if(parent->positions.size() > 0){
            posit = &parent->positions;
        }
        for(auto position: *posit){
            Position absPos;
            Position myRelPos = getPosition(position.time);
            absPos.time = position.time;
            absPos.x = position.x + myRelPos.x;
            absPos.y = position.y + myRelPos.y;
            absPos.z = position.z + myRelPos.z;
            absPositions.push_back(absPos);
        }
    } else {
        absPositions = positions;
    }
}

/**
 * Rotates positions (and hence movement) around 0, 0, 0 counter-clockwise
 */
void Positionable::rotatePositions(double degrees){
    if(positions.size() < 1){
        return;
    }
    for(auto position: positions){
        position.x = std::cos(degrees) * position.x;
        position.y = std::sin(degrees) * position.y;
    }
    if(parent != nullptr){
        centerPositionsAroundParent();
    }
}

static Position getPos(double time, std::vector<Position> &positions){
    if(positions.size() < 1){
        return Position(0,0,0,0);
    }
    Position *low = nullptr, *high = nullptr;
    double closeLow = 1000.0, closeHigh = 1000.0;
    for(auto pos : positions){
        if(time == pos.time){
            return pos;
        }
        if(pos.time < time && time - pos.time < closeLow){
            low = &pos;
            closeLow = time - pos.time;
        } else if(pos.time > time && pos.time - time < closeHigh){
            high = &pos;
            closeHigh = pos.time - time;
        }
    }
    if(low != nullptr && high == nullptr){
        return *low;
    } else if (low != nullptr && high != nullptr){
        Position position;
        double multTime = time - high->time;
        double travelTime = high->time - low->time;
        double travelX = high->x - low->x;
        double travelY = high->y - low->y;
        double travelZ = high->z - low->z;

        position.x = low->x + travelX * (multTime / travelTime);
        position.y = low->y + travelY * (multTime / travelTime);
        position.x = low->z + travelZ * (multTime / travelTime);
        position.time = time;
        return position;
    } else {
        std::cerr << "Error in positionable" << std::endl;
    }
    return Position();

}

/**
 * Computes position of positionable at a given time
 */
Position Positionable::getAbsPosition(double time){
    if(absPositions.size() > 0){
        return getPos(time, absPositions);
    } else if (absPositions.size() < 1 && parent != nullptr){
        centerPositionsAroundParent();
        return getPos(time, absPositions);
    } else {
        return getPos(time, positions); //No parent to move relative to
    }
}

/**
 * Computes position of positionable at a given time
 */
Position Positionable::getPosition(double time){
    return getPos(time, positions);
}