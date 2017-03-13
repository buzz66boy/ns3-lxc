#ifndef __SETTINGS_PARSER_CONF_H_INCLUDED__
#define __SETTINGS_PARSER_CONF_H_INCLUDED__

// forward declared dependencies

// include dependencies
#include <string>

// declarations

void create_template_settings_file(std::string settings_file);

class Settings {
public:
	static std::string ns3_path;
	static std::string output_dest;
	static std::string script_dest;

	static int parse_settings_file(std::string settings_file);
};

#endif
