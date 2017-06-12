#ifndef __COMMAND_PARSER_H_INCLUDED__
#define __COMMAND_PARSER_H_INCLUDED__

#include "yaml-cpp/yaml.h"

#include "topologyParser.h"

void parseCommands(YAML::Node cmds, ParsedTopology *parsedTop);

#endif