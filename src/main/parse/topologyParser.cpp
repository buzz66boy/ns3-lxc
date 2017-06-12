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
#include "errorCode.h"
#include "nodeParser.h"
#include "linkParser.h"
#include "ifaceParser.h"
#include "positionParser.h"
#include "applicationParser.h"
#include "commandParser.h"
#include "subTopologyParser.h"
#include "topologyParser.h"

using namespace std;

static void parseIncludes(YAML::Node includes, std::string topPath, ParsedTopology *parsedTop);
static void parseNodes(YAML::Node nodes, ParsedTopology *parsedTop);
static void parseLinks(YAML::Node links, ParsedTopology *parsedTop);
static void parseSubTopologies(YAML::Node topologies, ParsedTopology *parsedTop);\

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

	if(topology[TAG_TIME]){
		parsedTop.topology.runTime = topology[TAG_TIME].as<int>();
	}

	std::string topName = topPath.substr(topPath.find_last_of("\\/") + 1, topPath.find_last_of(".yaml") - topPath.find_last_of("\\/") - 5);
    bool topFound = false;
	if(topology[topName]){
		topology = topology[topName];
        topFound = true;
	} else if ('a' <= topName[0] <= 'z'){
		topName[0] = toupper(topName[0]);
		if(topology[topName]){
			topology = topology[topName];
            topFound = true;
		}
	} else if ('A' <= topName[0] <= 'Z'){
		topName[0] = tolower(topName[0]);
		if(topology[topName]){
			topology = topology[topName];
            topFound = true;
		}
	}

    if(!topFound){
        cout << "WARNING: Topology name declaration not found in " + topName + ", ignore this warning if using anonymous topology" << endl;
    }

	parsedTop.topology.name = topName;
    parsedTop.topology.origName = topName;
	parseTopology(topology, &parsedTop);
	return parsedTop.topology;
}

void parseTopology(YAML::Node topology, ParsedTopology *parsedTop){
	
	cout << "PARSING TOPOLOGY " << parsedTop->topology.name << endl;

	if(topology[TAG_NODE]){
		parseNodes(topology[TAG_NODE], parsedTop);
	} else if (topology[pluralize(TAG_NODE)]) {
		parseNodes(topology[pluralize(TAG_NODE)], parsedTop);
	}
	if(topology[TAG_TOPOLOGY]){
		parseSubTopologies(topology[TAG_TOPOLOGY], parsedTop);
	} else if (topology[pluralize(TAG_TOPOLOGY)]) {
		parseSubTopologies(topology[pluralize(TAG_TOPOLOGY)], parsedTop);
	}

	if(topology[TAG_LINK]){
		parseLinks(topology[TAG_LINK], parsedTop);
	} else if (topology[pluralize(TAG_LINK)]){
		parseLinks(topology[pluralize(TAG_LINK)], parsedTop);
	}

    if(topology[TAG_ACCEPT_IFACE]){
        parseAcceptedIfaces(topology[TAG_ACCEPT_IFACE], parsedTop);
    } else if (topology[pluralize(TAG_ACCEPT_IFACE)]){
        parseAcceptedIfaces(topology[pluralize(TAG_ACCEPT_IFACE)], parsedTop);
    }

	if(topology[TAG_IFACES_PROVIDED]){
		parseIfacesProvided(topology[TAG_IFACES_PROVIDED], parsedTop);
	}

	if(topology[TAG_IFACES_ACCEPTED]){
		parseIfacesAccepted(topology[TAG_IFACES_ACCEPTED], parsedTop);
	}

    if(topology[TAG_POSITION]){
        parsePositions(topology[TAG_POSITION], &parsedTop->topology);
    } else if (topology[pluralize(TAG_POSITION)]){
        parsePositions(topology[pluralize(TAG_POSITION)], &parsedTop->topology);
    }

    if(topology[TAG_ROTATION]){
        if(topology[TAG_ROTATION].Type() == YAML::NodeType::Scalar){
            applyRotation(topology[TAG_ROTATION].as<int>(), &parsedTop->topology);
        }
    }

	if(topology[TAG_APPLICATION]){
        parseApplications(topology[TAG_APPLICATION], parsedTop);
    } else if (topology[pluralize(TAG_APPLICATION)]){
        parseApplications(topology[pluralize(TAG_APPLICATION)], parsedTop);
    }

    if(topology[TAG_COMMAND]){
        parseCommands(topology[TAG_COMMAND], parsedTop);
    } else if(topology[pluralize(TAG_COMMAND)]){
        parseCommands(topology[pluralize(TAG_COMMAND)], parsedTop);
    }

    computeAbsolutePositions(&parsedTop->topology);
	cout << "DONE PARSING TOP " << parsedTop->topology.name << endl;

}

static void parseIncludes(YAML::Node includes, std::string topPath, ParsedTopology *parsedTop){
	//Get dir of top file to search for included files
	shared_ptr<ns3lxc::Topology> includedTop;
	std::string curInclude;
	std::string searchPath;
	std::string topDir = topPath.substr(0, topPath.find_last_of("\\/"));

    if(includes.Type() == YAML::NodeType::Scalar){
        curInclude = includes.as<std::string>();
        searchPath = topDir + "/" + curInclude + ".yaml";

        struct stat buffer;
        if(stat(searchPath.c_str(), &buffer) == 0 && !S_ISDIR(buffer.st_mode)){
            includedTop = shared_ptr<ns3lxc::Topology>(new ns3lxc::Topology(parseTopologyFile(searchPath)));
        } else {
            searchPath = searchPath = topDir + "/include/" + curInclude + ".yaml";
            if(stat(searchPath.c_str(), &buffer) == 0 && !S_ISDIR(buffer.st_mode)){
                includedTop = shared_ptr<ns3lxc::Topology>(new ns3lxc::Topology(parseTopologyFile(searchPath)));
            } else {
                throw Ns3lxcException(ErrorCode::FILE_NOT_FOUND, curInclude);
            }
        }

        parsedTop->nodes.insert(includedTop->nodeMap.begin(), includedTop->nodeMap.end());
        parsedTop->links.insert(includedTop->linkMap.begin(), includedTop->linkMap.end());
        parsedTop->includedTopologies[includedTop->name] = includedTop;
    } else {
    	for(auto i = 0; i < includes.size(); ++i){

    		curInclude = includes[i].as<std::string>();
    		searchPath = topDir + "/" + curInclude + ".yaml";

    		struct stat buffer;
    		if(stat(searchPath.c_str(), &buffer) == 0 && !S_ISDIR(buffer.st_mode)){
    			includedTop = shared_ptr<ns3lxc::Topology>(new ns3lxc::Topology(parseTopologyFile(searchPath)));
    		} else {
    			searchPath = searchPath = topDir + "/include/" + curInclude + ".yaml";
    			if(stat(searchPath.c_str(), &buffer) == 0 && !S_ISDIR(buffer.st_mode)){
    				includedTop = shared_ptr<ns3lxc::Topology>(new ns3lxc::Topology(parseTopologyFile(searchPath)));
    			} else {
                    throw Ns3lxcException(ErrorCode::FILE_NOT_FOUND, curInclude);
    			}
    		}

    		parsedTop->nodes.insert(includedTop->nodeMap.begin(), includedTop->nodeMap.end());
    		parsedTop->links.insert(includedTop->linkMap.begin(), includedTop->linkMap.end());
    		parsedTop->includedTopologies[includedTop->name] = includedTop;
    	}
    }
}

static void parseSubTopologies(YAML::Node topologies, ParsedTopology *parsedTop){
	for(auto i = 0; i < topologies.size(); ++i){
		std::vector<std::shared_ptr<ns3lxc::Topology> > curTops = parseSubTopology(topologies[i], parsedTop);
		for(shared_ptr<ns3lxc::Topology> curTop : curTops){
			parsedTop->topology.subTopologies.push_back(curTop);
			parsedTop->topology.topMap[curTop->name] = curTop;
		}
	}
	renameSubTopologies(&parsedTop->topology);
}

static void parseNodes(YAML::Node nodes, ParsedTopology *parsedTop){
	std::vector<shared_ptr<ns3lxc::Node> > curNodes;
	for(auto i = 0; i < nodes.size(); ++i){
		curNodes = parseNode(nodes[i], parsedTop);

		for(auto j = 0; j < curNodes.size(); ++j){
			if(parsedTop->topology.nodeMap.count(curNodes[j]->name) > 0){
				cout << "NODE EXISTS" << endl;
			} else {
				parsedTop->topology.nodeMap.insert(std::map<std::string, std::shared_ptr<ns3lxc::Node> >::value_type(curNodes[j]->name, curNodes[j]));
				parsedTop->topology.nodes.push_back(curNodes[j]);
				curNodes[j]->nodeNum = parsedTop->topology.curNodeNum++;
			}

		}
	}
}

static void parseLinks(YAML::Node links, ParsedTopology *parsedTop){
	for(auto i = 0; i < links.size(); ++i){
		parseLink(links[i], parsedTop);
	}
}

shared_ptr<ns3lxc::Node> findNode(vector<string> search, ns3lxc::Topology *top){
	if(search.size() < 1){
		throw Ns3lxcException(ErrorCode::NODE_NOT_FOUND, " find node");
	}
	if(top->topMap.count(search[0]) > 0){
		shared_ptr<ns3lxc::Topology> topPtr = top->topMap.at(search[0]);
		search.erase(search.begin());
		return findNode(search, topPtr.get());
	} else if(top->nodeMap.count(search[0]) > 0){
		return top->nodeMap.at(search[0]);
	} else {
		throw Ns3lxcException(ErrorCode::NODE_NOT_FOUND, search[0]);
	}
}

static void renameSubTopologies1(ns3lxc::Topology *topology, std::string prefix){
    if(prefix == ""){
        prefix = topology->origName + "_";
    } else {
        prefix = prefix + topology->origName + "_";
    }
    for(auto topPtr : topology->subTopologies){
        renameSubTopologies1(topPtr.get(), prefix);
    }

    for(auto nodePtr : topology->nodes){
        nodePtr->name = prefix + nodePtr->origName;
    }

    for(auto linkPtr : topology->links){
        linkPtr->name = prefix + linkPtr->origName;
    }
}

void renameSubTopologies(ns3lxc::Topology *topology, std::string prefix){
	for(auto subTopPtr : topology->subTopologies){
        renameSubTopologies1(subTopPtr.get(), prefix);
    }
}