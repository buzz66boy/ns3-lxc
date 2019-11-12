#include "lxcContainer.h"
#include "nodeTypeMap.h"

const std::map<std::string, NodeType*> nodeTypeMap = {
    {"ubuntu xenial", new LxcContainer("ubuntu", "xenial")}, 
    {"ubuntu bionic", new LxcContainer("ubuntu", "bionic")}, 
    {"archlinux current", new LxcContainer("archlinux", "current")}, 
    {"fedora 25", new LxcContainer("fedora", "25")},
};