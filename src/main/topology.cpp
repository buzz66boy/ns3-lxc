#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "topology.h"

using namespace std;

static void erase_char_from_string(string *str, char ch);

Topology parse_topology_file(string topology_path){
	std::ifstream top_file(topology_path.c_str(), std::ifstream::in);
	std::stringstream buffer;
	
	if(top_file.is_open()){
		buffer << top_file.rdbuf();
	}else{
		throw 8;
	}
	
	std::string full_topology(buffer.str());

	Topology topology;

	erase_char_from_string(&full_topology, '\n');
	erase_char_from_string(&full_topology, '\r');
	erase_char_from_string(&full_topology, '\t');
	
	cout << full_topology << endl;	
	
	return topology;
}

static void erase_char_from_string(string *str, char ch){
	str->erase(std::remove(str->begin(), str->end(), ch), str->end());
}
