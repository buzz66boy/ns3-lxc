#include <string>
#include <iostream>

#include "settings_conf.h"

#define SETTINGS_CONF_LOC "../../settings.conf"

using namespace std;

int main(int argc, char *argv[]){

	// open settings file and obtain directory locations
	int result = Settings::parse_settings_file(SETTINGS_CONF_LOC);
	if(result != 0){
		return result;
	}
	cout << Settings::ns3_path <<endl;	
	return 0;
}
