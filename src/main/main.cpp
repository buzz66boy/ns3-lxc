#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <sys/stat.h>

using namespace std;

int main(int argc, char *argv[]){
	// open settings file and obtain directory locations
	std::ifstream fileStream("../../settings.conf", std::ifstream::in);
	string line, ns3_path, script_dest, output_dest;
	
	if(fileStream.is_open()){
		while(getline(fileStream,line)){
			if(line.find("NS-3_PATH") != std::string::npos){
				ns3_path = line;	
			}else if(line.find("SCRIPT_DEST") != std::string::npos){
				script_dest = line;
			}else if(line.find("OUTPUT_DEST") != std::string::npos){
				output_dest = line;
			}
			
		}
		fileStream.close();
	}
	// check for required destinations and error if not found
	struct stat buffer;

	if(ns3_path == ""){
		cerr << "The NS-3 path variable is not set" << endl;
		return 1;
	}

	ns3_path = ns3_path.substr(ns3_path.find_first_of("=") + 1, ns3_path.length());
	script_dest = script_dest.substr(script_dest.find_first_of("=") + 1, script_dest.length());
	output_dest = output_dest.substr(output_dest.find_first_of("=") + 1, output_dest.length());	

	if(stat(ns3_path.c_str(), &buffer) != 0 || !S_ISDIR(buffer.st_mode)){
		cerr << "The NS-3 path specified does not exist or is not accesible" << endl;
		return 2;
	}
	return 0;
}
