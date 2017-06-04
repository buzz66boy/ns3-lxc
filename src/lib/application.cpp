#include <string>
#include "application.h"

using namespace ns3lxc;

Application::Application(const Application& temp): Nameable(temp), AdditionalTags(temp) {
    path = temp.path;
    args = temp.args;
}