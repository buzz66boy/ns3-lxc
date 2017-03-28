#include <string>
#include <vector>
#include <sstream>

#include "parserTags.h"

using namespace std;

std::string pluralize(std::string str){
    if(str == TAG_TOPOLOGY)
        return "topologies";
    else
        return str + "s";
}

std::vector<std::string> splitString(std::string str){
    std::vector<std::string> result;
    std::istringstream stream(str);
    for(std::string s; stream >> s; ){
        result.push_back(s);
    }
    return result;
}