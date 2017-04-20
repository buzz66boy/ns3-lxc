#ifndef __APPLICATION_TYPE_H_INCLUDED__
#define __APPLICATION_TYPE_H_INCLUDED__

#include <map>
#include <string>

#include "containerType.h"
#include "lxcContainerType.h"

static std::map<std::string, ContainerType*> containerTypeMap = {{"ubuntu xenial", new LxcContainerType("ubuntu", "xenial")}};

#endif