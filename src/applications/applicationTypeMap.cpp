#include <map>
#include <string>

#include "applicationType.h"
#include "genericPackman.h"
#include "quagga.h"
#include "applicationTypeMap.h"

const std::map<std::string, ApplicationType*> applicationTypeMap = {
    {"iperf3", new GenericPackman("iperf3", 3)},
    {"quagga", new Quagga()},
};