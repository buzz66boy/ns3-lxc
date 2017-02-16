#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <sys/stat.h>

#include "yaml-cpp/yaml.h"
#include "settings.h"

#define NS3_PATH_STR "NS-3_PATH"
#define SCRIPT_DEST_STR "SCRIPT_DEST"
#define OUTPUT_DEST_STR "OUTPUT_DEST"

#define SCRIPT_DEFAULT_DEST "../../output/scripts"
#define OUTPUT_DEFAULT_DEST "../../output"

#define MKDIR_MODE S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH

using namespace std;

void create_template_settings_file(string settings_file){
	std::ofstream file_stream(settings_file.c_str(), std::ios::binary);
	std::ifstream template_file("../../resources/settings_template.conf", std::ios::binary);
	
	file_stream << template_file.rdbuf();
	
	template_file.close();
	file_stream.close();
}

string Settings::ns3_path;
string Settings::script_dest;
string Settings::output_dest;

int Settings::parse_settings_file(std::string settings_file){
	// open settings file and obtain directory locations
	std::ifstream file_stream(settings_file.c_str(), std::ifstream::in);
	string line;
	bool found_path = false, found_script = false, found_output = false;
	int pos, comment_pos;

	YAML::Node config = YAML::LoadFile(settings_file);

	if(file_stream.is_open()){
		while(getline(file_stream,line) && (!found_path || !found_script || !found_output)){
			if(!found_path){
				pos = line.find(NS3_PATH_STR);
				comment_pos = line.find_first_of("#");
				if(pos != std::string::npos && (comment_pos == std::string::npos || pos < comment_pos)){
					ns3_path = line.substr(line.find_first_of(":") + 1, comment_pos);
					found_path = true;
					continue;
				}
			}
			if(!found_script){
				pos = line.find(SCRIPT_DEST_STR);
				comment_pos = line.find_first_of("#");
				if(pos != std::string::npos && (comment_pos == std::string::npos || pos < comment_pos)){
					script_dest = line.substr(line.find_first_of(":") + 1, comment_pos);
					found_script = true;
					continue;
				}				
			}
			if(!found_output){
				pos = line.find(OUTPUT_DEST_STR);
				comment_pos = line.find_first_of("#");
				if(pos != std::string::npos && (comment_pos == std::string::npos || pos < comment_pos)){
					output_dest = line.substr(line.find_first_of(":") + 1, comment_pos);
					found_output = true;
					continue;
				}				
			}
		}
		file_stream.close();
	} else {
		create_template_settings_file(settings_file);
		cerr << "No settings.conf file found, an empty template has been created in " << endl;
		return 1;
	}
	// check for required destinations and error if not found
	struct stat buffer;

	if(ns3_path == ""){
		cerr << "The NS-3 path variable is not set" << endl;
		return 2;
	}

	if(stat(ns3_path.c_str(), &buffer) != 0 || !S_ISDIR(buffer.st_mode)){
		cerr << "The NS-3 path specified does not exist or is not accesible" << endl;
		return 3;
	}

	if(!found_script){
		script_dest = SCRIPT_DEFAULT_DEST;
		int stat_result = stat(script_dest.c_str(), &buffer);
		if(stat_result != 0 || !S_ISDIR(buffer.st_mode)){
			if(mkdir(script_dest.c_str(), MKDIR_MODE) != 0){
				cerr << "Cannot make default script destination, aborting" << endl;
				return 4;
			}
		}
	}else{
		if(stat(script_dest.c_str(), &buffer) != 0 || !S_ISDIR(buffer.st_mode)){
			cerr << "Script destination in settings file not an accesible directory" << endl;
			return 5;
		}
	}
	
	if(!found_output){
		output_dest = OUTPUT_DEFAULT_DEST;
		int stat_result = stat(output_dest.c_str(), &buffer);
		if(stat_result != 0 || !S_ISDIR(buffer.st_mode)){
			if(mkdir(output_dest.c_str(), MKDIR_MODE) != 0){
				cerr << "Cannot make default output destination, aborting" << endl;
				return 6;
			}
		}
	}else{
		if(stat(output_dest.c_str(), &buffer) != 0 || !S_ISDIR(buffer.st_mode)){
			cerr << "Output destination in settings file not an accesible directory" << endl;
			return 7;
		}
	}

	return 0;
}
