#include <string>
#include <iostream>

#include "settings_conf.h"
#include "topology.h"

#define SETTINGS_CONF_LOC "../../settings.conf"
#define NO_FILE_PROVIDED "Exiting, no file provided"

using namespace std;

int main(int argc, char *argv[]){

	// open settings file and obtain directory locations
	int result = Settings::parse_settings_file(SETTINGS_CONF_LOC);
	if(result != 0){
		return result;
	}
	
	Topology topology;
	
	if(argc > 1){
		topology = parse_topology_file(argv[1]);
	}else{
		cerr << NO_FILE_PROVIDED << endl;
		return 1;
	}
	return 0;
	
}
