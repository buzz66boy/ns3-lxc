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

Topology parse_topology_file(std::string topology_path);

class Topology {
	std::vector<Container> containers;
	std::vector<Network> networks;
	std::map<Interface, Container> if_to_cont;
};

class Container {
	std::string name;
	std::vector<Interface> interfaces;
	std::vector<Position> positions;
};

class Interface {
	std::string name;
	int *ip;
};

class Position {
	int time;
	int x;
	int y;
	int z;
};

class Network {
	std::string name;
	int type;
	std::vector<std::string> connected;
};

#endif
