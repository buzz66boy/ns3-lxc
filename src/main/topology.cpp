#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <arpa/inet.h>

#include "topology.h"

using namespace std;

static void erase_char_from_string(string *str, char ch);

static void strip_comments(string *str);

// for parsing a structure, string returned is remainder of string passed in
static string parse_to_container(Topology *top, string *str);
static string parse_to_interface(Topology *top, Interfaceable interf, string *str);
static string parse_to_position(Topology *top, Positionable positionable, string *str);
static string parse_to_network(Topology *top, string *str);

Topology parse_topology_file(string topology_path){
	std::ifstream top_file(topology_path.c_str(), std::ifstream::in);
	std::stringstream buffer;
	
	if(top_file.is_open()){
		buffer << top_file.rdbuf();
	}else{
		throw 8;
	}

	string full_topology(buffer.str());

	Topology topology;

	erase_char_from_string(&full_topology, '\n');
	erase_char_from_string(&full_topology, '\r');
	erase_char_from_string(&full_topology, '\t');
	
	strip_comments(&full_topology);
	
	size_t start_tag_loc = full_topology.find(FLAG_START);
	size_t end_tag_loc = full_topology.find(FLAG_END) + sizeof(FLAG_END) - 1;
	string flag_str = full_topology.substr(start_tag_loc, end_tag_loc - start_tag_loc);
	string rest_of_top = full_topology.substr(end_tag_loc, full_topology.length() - end_tag_loc );
	string flag_field = rest_of_top.substr(0, rest_of_top.find(FLAG_START));
	
	do {
		if(flag_str.find(FLAG_TERMINATE_START) != string::npos){
			// do nothing
			if(flag_str.find(TAG_TOPOLOGY)) {
				break;
			}

		} else if(flag_str.find(TAG_CONTAINER) != string::npos){
			rest_of_top = parse_to_container(&topology, &rest_of_top);

			cout << "container parsing" << endl;

		}else if(flag_str.find(TAG_NETWORK) != string::npos){
			rest_of_top = parse_to_network(&topology, &rest_of_top);
			
			cout << "network parsing" << endl;
		}
		
		start_tag_loc = rest_of_top.find(FLAG_START);
		end_tag_loc = rest_of_top.find(FLAG_END) + sizeof(FLAG_END) - 1;
		flag_str = rest_of_top.substr(start_tag_loc, end_tag_loc - start_tag_loc);
		rest_of_top = rest_of_top.substr(end_tag_loc, rest_of_top.length() - end_tag_loc);
		flag_field = rest_of_top.substr(0, rest_of_top.find(FLAG_START));

	} while (start_tag_loc != std::string::npos && end_tag_loc != std::string::npos);
	
//	cout << topology.containers[0].name << endl;
	
	return topology;
}

static void erase_char_from_string(string *str, char ch){
	str->erase(std::remove(str->begin(), str->end(), ch), str->end());
}

static void strip_comments(string *str){
	size_t comment_loc = str->find(TAG_START_COMMENT);
	size_t end_comment_loc = str->find(TAG_END_COMMENT) + sizeof(TAG_END_COMMENT) - 1;
	
	while(comment_loc != std::string::npos){
		str->erase(comment_loc, end_comment_loc - comment_loc);
		comment_loc = str->find(TAG_START_COMMENT);
		end_comment_loc = str->find(TAG_END_COMMENT) + sizeof(TAG_END_COMMENT) - 1;
		
	}
}

static string parse_to_container(Topology *top, string *str){
	Container cont;

	size_t start_tag_loc = str->find(FLAG_START);
	size_t end_tag_loc = str->find(FLAG_END) + sizeof(FLAG_END) - 1;
	string flag_str = str->substr(start_tag_loc, end_tag_loc - start_tag_loc );
	string rest_of_top = str->substr(end_tag_loc, str->length() - end_tag_loc );
	string flag_field = rest_of_top.substr(0, rest_of_top.find(FLAG_START));
	
	do {

		if(flag_str.find(FLAG_TERMINATE_START) != string::npos && flag_str.find(TAG_CONTAINER) != string::npos){
			break;
		}

		if(flag_str.find(FLAG_TERMINATE_START) != string::npos){

		} else if(flag_str.find(TAG_COMMON_NAME) != string::npos){
			cont.name = flag_field;
		}else if(flag_str.find(TAG_CONTAINER_INTERFACE) != string::npos){
			cout << "Container Interface" << endl;
		}else if(flag_str.find(TAG_CONTAINER_POSITION) != string::npos){
			cout << "Container Position" << endl;
		}else if(flag_str.find(TAG_CONTAINER_OS) != string::npos){
			cout << "Container OS" << endl;
		}

		start_tag_loc = rest_of_top.find(FLAG_START);
		end_tag_loc = rest_of_top.find(FLAG_END) + sizeof(FLAG_END) - 1;
		flag_str = rest_of_top.substr(start_tag_loc, end_tag_loc - start_tag_loc );
		rest_of_top = rest_of_top.substr(end_tag_loc, rest_of_top.length() - end_tag_loc);
		
		flag_field = rest_of_top.substr(0, rest_of_top.find(FLAG_START));
		
	} while (start_tag_loc != std::string::npos && end_tag_loc != std::string::npos);

	top->containers.push_back(cont);
	cout << cont.name << endl;
	return rest_of_top;
}

static string parse_to_interface(Topology *top, Interfaceable interf, string *str){
	Interface inf;

	size_t start_tag_loc = str->find(FLAG_START);
	size_t end_tag_loc = str->find(FLAG_END) + sizeof(FLAG_END) - 1;
	string flag_str = str->substr(start_tag_loc, end_tag_loc - start_tag_loc );
	string rest_of_top = str->substr(end_tag_loc, str->length() - end_tag_loc );
	string flag_field = rest_of_top.substr(0, rest_of_top.find(FLAG_START));
	
	do {

		if(flag_str.find(FLAG_TERMINATE_START) != string::npos && flag_str.find(TAG_CONTAINER_INTERFACE) != string::npos){
			break;
		}

		if(flag_str.find(FLAG_TERMINATE_START) != string::npos){

		} else if(flag_str.find(TAG_COMMON_NAME) != string::npos){
			inf.name = flag_field;
		}else if(flag_str.find(TAG_INTERFACE_IP) != string::npos){
			cout << "Interface Ip" << endl;
			inet_pton(AF_INET, flag_field.c_str(), &inf.ip);
		}

		start_tag_loc = rest_of_top.find(FLAG_START);
		end_tag_loc = rest_of_top.find(FLAG_END) + sizeof(FLAG_END) - 1;
		flag_str = rest_of_top.substr(start_tag_loc, end_tag_loc - start_tag_loc );
		rest_of_top = rest_of_top.substr(end_tag_loc, rest_of_top.length() - end_tag_loc);
		
		flag_field = rest_of_top.substr(0, rest_of_top.find(FLAG_START));
		
	} while (start_tag_loc != std::string::npos && end_tag_loc != std::string::npos);

	interf.interfaces.push_back(inf);
	
	if(inf.name == ""){
		inf.name = "temp_name";
	}
	
	top->if_to_inf.insert({inf.name, interf});
	return rest_of_top;
}

static string parse_to_position(Topology *top, Positionable positionable, string *str){
	Position pos();
	return *str;
}

static string parse_to_network(Topology *top, string *str){
	Network net();
	return *str;
}
