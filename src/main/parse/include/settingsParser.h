#ifndef __SETTINGS_PARSER_CONF_H_INCLUDED__
#define __SETTINGS_PARSER_CONF_H_INCLUDED__

// forward declared dependencies

// include dependencies
#include <string>

// declarations
enum Mode {
    NONE = 0,
    PARSE = 1<<0,
    NODE_GEN = 1<<1,
    NS3_GEN = 1<<2, 
    NS3_RUN = 1<<3,
    CLEANUP = 1<<4,
    NORMAL = 0xffff,
};

void create_template_settings_file(std::string settings_file);
bool check_make_dir(const char *path);

class Settings {
public:
    static int run_mode;
    static bool gdb;
	static std::string ns3_path;
	static std::string output_dest;
    //Location output for the topology should be saved
    static std::string top_output_dest;
    static std::string temp_dir;
    static std::string container_config_dir;
	static std::string script_dest;
    static std::string node_type;
    static double run_time;

    // static int parse_config_args();
	static int parse_settings_file(std::string settings_file);

    static bool genContainers(){ return (run_mode & Mode::NODE_GEN); }
    static bool genNS3() { return (run_mode & Mode::NS3_GEN); }
    static bool runNS3() { return (run_mode & Mode::NS3_RUN); }
    static bool gdbNS3() { return gdb; }
    static bool teardown() { return (run_mode & Mode::CLEANUP); }
};

#endif
