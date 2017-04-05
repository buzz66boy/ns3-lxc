#include <string>
#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <memory>

#include <arpa/inet.h>
#include <sys/stat.h>

#include "yaml-cpp/yaml.h"

#include "topology.h"
#include "node.h"
#include "link.h"
#include "iface.h"
#include "position.h"
#include "parserTags.h"
#include "nodeParser.h"
#include "linkParser.h"
#include "subTopologyParser.h"
#include "topologyParser.h"

using namespace std;

static void parseIncludes(YAML::Node includes, std::string topPath, ParsedTopology *parsedTop);
static void parseNodes(YAML::Node nodes, ParsedTopology *parsedTop);
static void parseLinks(YAML::Node links, ParsedTopology *parsedTop);
static void parseSubTopologies(YAML::Node topologies, ParsedTopology *parsedTop);
static void parseIfacesProvided(YAML::Node ifaces, ParsedTopology *parsedTop);
static void parseIfacesAccepted(YAML::Node ifacesAccepted, ParsedTopology *parsedTop);
static ns3lxc::Iface parseInterface(YAML::Node interface);

ns3lxc::Topology parseTopologyFile(std::string topPath){
	ParsedTopology parsedTop;
	
	YAML::Node topology = YAML::LoadFile(topPath);

	if(topology[TAG_INCLUDE]){
		cout << "Parsing includes" << endl;
		parseIncludes(topology[TAG_INCLUDE], topPath, &parsedTop);
	} else if (topology[pluralize(TAG_INCLUDE)]){
		cout << "Parsing includes" << endl;
		parseIncludes(topology[pluralize(TAG_INCLUDE)], topPath, &parsedTop);
	}

	std::string topName = topPath.substr(topPath.find_last_of("\\/") + 1, topPath.find_last_of(".yaml") - topPath.find_last_of("\\/") - 5);

	if(topology[topName]){
		topology = topology[topName];
	} else if ('a' <= topName[0] <= 'z'){
		topName[0] = toupper(topName[0]);
		if(topology[topName]){
			topology = topology[topName];
		}else{
			
		}
	} else if ('A' <= topName[0] <= 'Z'){
		topName[0] = tolower(topName[0]);
		if(topology[topName]){
			topology = topology[topName];
		}else{

		}
	}

	if(topology[TAG_NODE]){
		parseNodes(topology[TAG_NODE], &parsedTop);
	} else if (topology[pluralize(TAG_NODE)]) {
		parseNodes(topology[pluralize(TAG_NODE)], &parsedTop);
	}
	if(topology[TAG_TOPOLOGY]){
		parseSubTopologies(topology[TAG_TOPOLOGY], &parsedTop);
	} else if (topology[pluralize(TAG_TOPOLOGY)]) {
		parseSubTopologies(topology[pluralize(TAG_TOPOLOGY)], &parsedTop);
	}

	if(topology[TAG_LINK]){
		parseLinks(topology[TAG_LINK], &parsedTop);
	} else if (topology[pluralize(TAG_LINK)]){
		parseLinks(topology[pluralize(TAG_LINK)], &parsedTop);
	}

	if(topology[TAG_IFACES_PROVIDED]){
		parseIfacesProvided(topology[TAG_IFACES_PROVIDED], &parsedTop);
	}

	if(topology[TAG_IFACES_ACCEPTED]){
		parseIfacesAccepted(topology[TAG_IFACES_ACCEPTED], &parsedTop);
	}

	return parsedTop.topology;
}

static void parseIncludes(YAML::Node includes, std::string topPath, ParsedTopology *parsedTop){
	//Get dir of top file to search for included files
	ns3lxc::Topology includedTop;
	std::string curInclude;
	std::string searchPath;
	std::string topDir = topPath.substr(0, topPath.find_last_of("\\/"));

	for(auto i = 0; i < includes.size(); ++i){

		curInclude = includes[i].as<std::string>();
		searchPath = topDir + "/" + curInclude + ".yaml";

		struct stat buffer;
		if(stat(searchPath.c_str(), &buffer) == 0 && !S_ISDIR(buffer.st_mode)){
			includedTop = parseTopologyFile(searchPath);
		} else {
			searchPath = searchPath = topDir + "/include/" + curInclude + ".yaml";
			if(stat(searchPath.c_str(), &buffer) == 0 && !S_ISDIR(buffer.st_mode)){
				includedTop = parseTopologyFile(searchPath);
			} else {
				cerr << "Couldn't find included file " << curInclude << " while parsing " << topPath << endl;
				exit(10);
			}
		}

		parsedTop->nodes.insert(includedTop.nodeMap.begin(), includedTop.nodeMap.end());
		parsedTop->links.insert(includedTop.linkMap.begin(), includedTop.linkMap.end());
		parsedTop->includedTopologies[curInclude] = includedTop;
	}
}

static void parseSubTopologies(YAML::Node topologies, ParsedTopology *parsedTop){
	for(auto i = 0; i < topologies.size(); ++i){
		std::vector<std::shared_ptr<ns3lxc::Topology> > curTops = parseSubTopology(topologies[i], parsedTop);
		for(auto curTop : curTops){
			parsedTop->topology.subTopologies.push_back(curTop);
			parsedTop->topology.topMap[curTop->name] = curTop;
		}
	}
}

static void parseNodes(YAML::Node nodes, ParsedTopology *parsedTop){
	std::vector<shared_ptr<ns3lxc::Node> > curNodes;
	for(auto i = 0; i < nodes.size(); ++i){
		curNodes = parseNode(nodes[i], parsedTop);
		parsedTop->topology.nodes.insert(parsedTop->topology.nodes.end(), curNodes.begin(), curNodes.end());
		for(auto j = 0; j < curNodes.size(); ++j){
			parsedTop->topology.nodeMap.insert(std::map<std::string, std::shared_ptr<ns3lxc::Node> >::value_type(curNodes[j]->name, curNodes[j]));
		}
	}
}

static void parseLinks(YAML::Node links, ParsedTopology *parsedTop){
	for(auto i = 0; i < links.size(); ++i){
		shared_ptr<ns3lxc::Link> curLink = parseLink(links[i], parsedTop);
		parsedTop->topology.links.push_back(curLink);
		parsedTop->topology.linkMap[curLink->getName()] = curLink;
	}
}

static void parseIfacesProvided(YAML::Node ifaces, ParsedTopology *parsedTop){

}

static void parseIfacesAccepted(YAML::Node ifacesAccepted, ParsedTopology *parsedTop){

}