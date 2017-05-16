#include <string>
#include <iostream>
#include <cstring>
#include <map>
#include <unistd.h>

#include "yaml-cpp/yaml.h"
#include "settingsParser.h"
#include "topologyParser.h"
#include "topologyGenerator.h"
#include "topology.h"
#include "node.h"
#include "link.h"

#define MAXPATHLEN 1024

#define PROJ_ROOT_DIR "NS-3_LXC"
#define SETTINGS_FILE "settings.yaml"
#define NO_FILE_PROVIDED "Exiting, no file provided"

using namespace std;

static std::string compute_settings_path(){
	char cwd[MAXPATHLEN];
	size_t substr;
	std::string dir;

	getcwd(cwd, MAXPATHLEN);
	dir = std::string(cwd);
	
	substr = dir.find(PROJ_ROOT_DIR);

	if(substr != std::string::npos){
		dir = dir.substr(0, substr + strlen(PROJ_ROOT_DIR));
		dir = dir + '/' + SETTINGS_FILE;
	} else {
		dir = SETTINGS_FILE;
	}

	return dir;
}

static map<string, string> parseArgs(int argc, char *argv[]){
	map<string, string> argMap;
	for(int i = 1; i < argc; ++i){
		string arg(argv[i]);
		if(arg.find('-') != string::npos){
			argMap[arg] = "exists";
		} else {
			argMap["file"] = arg;
			cout << arg << endl;
		}
	}
	return argMap;
}

int main(int argc, char *argv[]){

	if(geteuid()){
		cerr << "Must run with root privileges" << endl;
		exit(-1);
	}
	
	std::string settings_path = compute_settings_path();

	// open settings file and obtain directory locations
	int result = Settings::parse_settings_file(settings_path);
	if(result != 0){
		return result;
	}
	if(argc < 2) {
		cerr << "Not Enough Arguments" << endl;
		return 1;
	}
	map<string, string> argMap = parseArgs(argc, argv);
	ns3lxc::Topology topology;

	if(argMap.count("file") < 1) {
		cerr << NO_FILE_PROVIDED << endl;
		return 1;
	}

	topology = parseTopologyFile(argMap.at("file"));
	ns3lxc::Topology::reNumNodes(&topology);
	if(argMap.count("-n") > 0){
		Settings::run_mode = Mode::NS3_GEN;
		cout << "RUN mode ns3" << endl;
	} else if(argMap.count("-c") > 0){
		Settings::run_mode = Mode::CLEANUP;
	}

	generateTopology(&topology);

	return 0;
	
}
