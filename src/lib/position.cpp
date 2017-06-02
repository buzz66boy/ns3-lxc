#include <cmath>
#include <iostream>
#include <map>

#include "errorCode.h"
#include "position.h"

#define PI 3.141592653

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
        std::map<double, bool> times;
        for(auto position: positions){
            times[position.time] = false;
        }
        for(auto position: *posit){
            Position absPos;
            Position myRelPos = getPosition(position.time);
            absPos.time = position.time;
            times[position.time] = true;
            absPos.x = position.x + myRelPos.x;
            absPos.y = position.y + myRelPos.y;
            absPos.z = position.z + myRelPos.z;
            absPositions.push_back(absPos);
        }
        for(auto pos: positions){
            if(!times.at(pos.time)){
                Position absPos;
                Position myRelPos = getPosition(pos.time);
                Position parentPos = parent->getAbsPosition(pos.time);
                absPos.time = parentPos.time;
                times[parentPos.time] = true;
                absPos.x = parentPos.x + myRelPos.x;
                absPos.y = parentPos.y + myRelPos.y;
                absPos.z = parentPos.z + myRelPos.z;
                absPositions.push_back(absPos);
            }
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
    for(int i = 0; i < positions.size(); ++i){
        double x = positions[i].x;
        double y = positions[i].y;
        double curAngle = 0.0;
        double hypotenus = std::sqrt(x*x + y*y);
        if(x != 0){
            curAngle = std::atan(y/x);
        }
        positions[i].x = std::cos(curAngle + (PI * degrees) / 180.0) * hypotenus;
        positions[i].y = std::sin(curAngle + (PI * degrees) / 180.0) * hypotenus;
    }
    if(parent != nullptr){
        centerPositionsAroundParent();
    }
}

static Position getPos(double time, std::vector<Position> positions){
    if(positions.size() < 1){
        return Position(time,0,0,0);
    }
    Position *low = nullptr, *high = nullptr;
    double closeLow = 10000.0, closeHigh = 10000.0;
    for(int i = 0; i < positions.size(); ++i){
        if(time == positions[i].time){
            return positions[i];
        }
        if(positions[i].time < time && time - positions[i].time < closeLow){
            low = &positions[i];
            closeLow = time - positions[i].time;
        } else if(positions[i].time > time && positions[i].time - time < closeHigh){
            high = &positions[i];
            closeHigh = positions[i].time - time;
        }
    }
    if(low != nullptr && high == nullptr){
        Position ret(*low);
        ret.time = time;
        return ret;
    } else if (low != nullptr && high != nullptr){
        Position position;
        double multTime = time - low->time;
        double travelTime = high->time - low->time;
        double travelX = high->x - low->x;
        double travelY = high->y - low->y;
        double travelZ = high->z - low->z;

        position.x = low->x + travelX * (multTime / travelTime);
        position.y = low->y + travelY * (multTime / travelTime);
        position.z = low->z + travelZ * (multTime / travelTime);
        position.time = time;
        return position;
    }

    throw Ns3lxcException(ErrorCode::POSITION_ERROR, std::to_string(time));

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