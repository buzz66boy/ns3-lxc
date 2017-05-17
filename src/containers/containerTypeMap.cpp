#include "lxcContainerType.h"
#include "containerTypeMap.h"

const std::map<std::string, ContainerType*> containerTypeMap = {{"ubuntu xenial", new LxcContainerType("ubuntu", "xenial")}};