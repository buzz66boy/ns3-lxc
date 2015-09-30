#ifndef __TOPOLOGY_H_INCLUDED__
#define __TOPOLOGY_H_INCLUDED__

// forward declared dependencies

class Interface;
class Position;
class Network;
class Container;
class Topology;

// include dependencies
#include <string>
#include <vector>
#include <map>

// declarations

#define FLAG_START "<"
#define FLAG_END ">"
#define FLAG_TERMINATE_START "</"

#define TAG_START_COMMENT "<!--"
#define TAG_END_COMMENT "-->"

#define TAG_TOPOLOGY "topology" 
#define TAG_CONTAINER "container" 
#define TAG_NETWORK "network"

#define TAG_COMMON_NAME "name"

#define TAG_CONTAINER_INTERFACE "interface"
#define TAG_CONTAINER_POSITION "position"
#define TAG_CONTAINER_OS "os"

#define TAG_NETWORK_TYPE "type"
#define TAG_NETWORK_CONNECTED "connected"

Topology parse_topology_file(std::string topology_path);

//interfaces
class Nameable {
public:
	std::string name;
};

class Interfaceable {
public:
	std::vector<Interface> interfaces;
};

class Positionable {
public:
	std::vector<Position> positions;
};

//classes
class Topology {
public:
	std::vector<Container> containers;
	std::vector<Network> networks;
	std::vector<Position> time_sorted_positions;
	std::map<Interface, Container> if_to_cont;
};

class Container: public Nameable, public Interfaceable, public Positionable {};

class Interface: public Nameable {
public:
	int *ip;
};

class Position {
public:
	Container *container;
	int time;
	int x;
	int y;
	int z;
};

class Network: public Nameable, public Interfaceable {
public:
	int type;
	std::vector<std::string> connected;
};

#endif
