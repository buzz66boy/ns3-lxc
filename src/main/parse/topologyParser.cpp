#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <arpa/inet.h>

#include "yaml-cpp/yaml.h"

#include "topology.h"
#include "node.h"
#include "connection.h"
#include "iface.h"
#include "position.h"
#include "topologyParser.h"
#include <sys/stat.h>

using namespace std;

static ParsedTopology parseIncludes(YAML::Node includes, std::string topPath, ParsedTopology parsedTop);

// for parsing a structure, string returned is remainder of string passed in
static ns3lxc::Topology parseTopology(YAML::Node topology);
static ns3lxc::Node parseNode(YAML::Node node);
static ns3lxc::Iface parseInterface(YAML::Node interface);

static std::string pluralize(std::string str){
	return str + "s";
}

ns3lxc::Topology parseTopologyFile(std::string topPath){
	ParsedTopology parsedTop;
	
	YAML::Node topology = YAML::LoadFile(topPath);

	if(topology[TAG_INCLUDE]){
		parsedTop = parseIncludes(topology[TAG_INCLUDE], topPath, parsedTop);
	} else if (topology[pluralize(TAG_INCLUDE)]){
		parsedTop = parseIncludes(topology[pluralize(TAG_INCLUDE)], topPath, parsedTop);
	}

	return parsedTop.topology;
}

static ParsedTopology parseIncludes(YAML::Node includes, std::string topPath, ParsedTopology parsedTop){
	//Get dir of top file to search for included files
	ns3lxc::Topology includedTop;
	size_t i, j;
	std::string curInclude;
	std::string searchPath;
	std::string topDir = topPath.substr(0, topPath.find_last_of("\\/"));

	for(i = 0; i < includes.size(); ++i){

		curInclude = includes[i].as<std::string>();
		searchPath = topDir + "/" + curInclude + ".yaml";

		struct stat buffer;
		if(stat(searchPath.c_str(), &buffer) == 0 && !S_ISDIR(buffer.st_mode)){
			cout << "I found inc " << curInclude << " @ " << searchPath << endl;
			includedTop = parseTopologyFile(searchPath);
		} else {
			searchPath = searchPath = topDir + "/include/" + curInclude + ".yaml";
			if(stat(searchPath.c_str(), &buffer) == 0 && !S_ISDIR(buffer.st_mode)){
				cout << "I found inc " << curInclude << " @ " << searchPath << endl;
				includedTop = parseTopologyFile(searchPath);
			} else {
				cerr << "Couldn't find included file " << curInclude << " while parsing " << topPath << endl;
				exit(10);
			}
		}

		// parsedTop.nodes.insert(includedTop.nodeMap.begin(), includedTop.nodeMap.end());
		// parsedTop.connections.insert(includedTop.connectionMap.begin(), includedTop.connectionMap.end());

	}
	return parsedTop;
}

static ns3lxc::Topology parseTopology(YAML::Node topology){
	ns3lxc::Topology top;
	
	return top;
}

static ns3lxc::Node parseNode(YAML::Node node){
	ns3lxc::Node nd;

	
	return nd;
}

static ns3lxc::Iface parseInterface(YAML::Node interface){
	ns3lxc::Iface iface;
	return iface;
}
