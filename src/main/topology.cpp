#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <arpa/inet.h>

#include "yaml-cpp/yaml.h"

#include "network.h"
#include "container.h"
#include "connection.h"
#include "iface.h"
#include "position.h"
#include "topology.h"

using namespace std;

// for parsing a structure, string returned is remainder of string passed in
static ns3lxc::Network parseNetwork(YAML::Node network);
static ns3lxc::Container parseContainer(YAML::Node container);
static ns3lxc::Iface parseInterface(YAML::Node interface);

static std::string pluralize(std::string str){
	return str + "s";
}

Topology parseTopology(string topology_path){
	Topology top;
	
	YAML::Node topology = YAML::LoadFile(topology_path);

	return top;
}

static ns3lxc::Network parseNetwork(YAML::Node network){
	ns3lxc::Network net;
	
	return net;
}

static ns3lxc::Container parseContainer(YAML::Node container){
	ns3lxc::Container cont;

	
	return cont;
}

static ns3lxc::Iface parseInterface(YAML::Node interface){
	ns3lxc::Iface iface;
	return iface;
}
