#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <sys/stat.h>

#include "yaml-cpp/yaml.h"
#include "errorCode.h"
#include "settingsParser.h"

#define NS3_PATH_STR "NS-3_PATH"
#define SCRIPT_DEST_STR "SCRIPT_DEST"
#define TEMP_DEST_STR "TEMP_DIR"
#define OUTPUT_DEST_STR "OUTPUT_DEST"
#define NODE_TYPE "NODE_TYPE"
#define SIM_TIME "SIM_TIME"

#define SCRIPT_DEFAULT_DEST "../../output/scripts"
#define OUTPUT_DEFAULT_DEST "../../output"
#define DEFAULT_NODE_TYPE "ubuntu xenial"
#define DEFAULT_SIM_TIME 60.0

#define CONTAINER_CONFIG "/container_config"

#define MKDIR_MODE S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH

using namespace std;

void create_template_settings_file(string settings_file){
	std::ofstream file_stream(settings_file.c_str(), std::ios::binary);
	std::ifstream template_file("../../resources/settings_template.conf", std::ios::binary);
	
	file_stream << template_file.rdbuf();
	
	template_file.close();
	file_stream.close();
}

int Settings::run_mode = Mode::NORMAL;
bool Settings::gdb = false;
string Settings::ns3_path;
string Settings::script_dest;
string Settings::temp_dir;
string Settings::top_output_dest;
string Settings::output_dest;
string Settings::container_config_dir;
string Settings::node_type;
double Settings::run_time;

bool check_make_dir(const char *path){
	struct stat buffer;
	int stat_result = stat(path, &buffer);
	if(stat_result != 0 || !S_ISDIR(buffer.st_mode)){
		if(mkdir(path, MKDIR_MODE) != 0){
			return false;
		}
	}
	return true;
}

int Settings::parse_settings_file(std::string settings_file){
	// open settings file and obtain directory locations
	std::ifstream file_stream(settings_file.c_str(), std::ifstream::in);
	string line;
	bool found_path = false, found_script = false, found_output = false;
	int pos, comment_pos, stat_result;

	YAML::Node settings = YAML::LoadFile(settings_file);

	if(!settings){
		create_template_settings_file(settings_file);
		throw Ns3lxcException(ErrorCode::FILE_NOT_FOUND, settings_file);
	}

	if(settings[NS3_PATH_STR]){
		Settings::ns3_path = settings[NS3_PATH_STR].as<std::string>();
	} else {
		throw Ns3lxcException(ErrorCode::SETTINGS_ATTR_NOT_FOUND, NS3_PATH_STR);
	}

	// check for required destinations and error if not found
	struct stat buffer;
	if(stat(Settings::ns3_path.c_str(), &buffer) != 0 || !S_ISDIR(buffer.st_mode)){
		throw Ns3lxcException(ErrorCode::FOLDER_NOT_FOUND, Settings::ns3_path);
	}

	if(settings[TEMP_DEST_STR]){
		Settings::temp_dir = settings[TEMP_DEST_STR].as<string>();
	} else {
		throw Ns3lxcException(ErrorCode::SETTINGS_ATTR_NOT_FOUND, TEMP_DEST_STR);
	}

	if(!check_make_dir(Settings::temp_dir.c_str())){
		throw Ns3lxcException(ErrorCode::FOLDER_NOT_CREATED, Settings::temp_dir);
	}

	if(settings[SCRIPT_DEST_STR]){
		Settings::script_dest = settings[SCRIPT_DEST_STR].as<std::string>();
	} else {
		Settings::script_dest = SCRIPT_DEFAULT_DEST;
	}

	if(!check_make_dir(Settings::script_dest.c_str())){
		throw Ns3lxcException(ErrorCode::FOLDER_NOT_CREATED, Settings::script_dest);
	}

	if(settings[OUTPUT_DEST_STR]){
		Settings::output_dest = settings[OUTPUT_DEST_STR].as<std::string>();
	} else {
		Settings::output_dest = OUTPUT_DEFAULT_DEST;
	}

	if(!check_make_dir(Settings::output_dest.c_str())){
		throw Ns3lxcException(ErrorCode::FOLDER_NOT_CREATED, Settings::output_dest);
	}

	Settings::container_config_dir = Settings::temp_dir + CONTAINER_CONFIG;
	if(!check_make_dir(Settings::container_config_dir.c_str())){
		throw Ns3lxcException(ErrorCode::FOLDER_NOT_CREATED, Settings::container_config_dir);
	}
	
	if(settings[NODE_TYPE]){
		Settings::node_type = settings[NODE_TYPE].as<std::string>();
	} else {
		Settings::node_type = DEFAULT_NODE_TYPE;
	}

	if(settings[SIM_TIME]){
		Settings::run_time = settings[SIM_TIME].as<double>();
	} else { 
		Settings::run_time = DEFAULT_SIM_TIME;
	}

	return 0;
}
