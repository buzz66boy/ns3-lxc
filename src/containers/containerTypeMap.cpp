#include "lxcContainerType.h"
#include "containerTypeMap.h"

const std::map<std::string, ContainerType*> containerTypeMap = {
    {"ubuntu xenial", new LxcContainerType("ubuntu", "xenial")}, 
    {"archlinux current", new LxcContainerType("archlinux", "current")}, 
    {"fedora 25", new LxcContainerType("fedora", "25")},
};