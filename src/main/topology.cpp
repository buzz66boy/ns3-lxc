#include <string>
#include <iostream>
#include <fstream>
#include "topology.h"

using namespace std;

static Topology parse_topology_file(string topology_path){
	std::ifstream top_file(topology_path.c_str(), std::ifstream::in);
	string line;
	
	if(top_file.is_open()){
		while(getline(top_file, line)){
			
		}
	}

	Topology topology;
	
	return topology;
}
