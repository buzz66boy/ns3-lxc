#ifndef __APPLICATION_PARSER_H_INCLUDED__
#define __APPLICATION_PARSER_H_INCLUDED__

#include "yaml-cpp/yaml.h"

#include "topologyParser.h"

void parseApplications(YAML::Node apps, ParsedTopology *parsedTop);

#endif