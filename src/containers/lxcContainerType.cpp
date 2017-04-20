#include <memory>
#include <iostream>

#include <lxc/lxccontainer.h>

#include "node.h"
#include "lxcContainerType.h"

using namespace std;

/**
 * https://github.com/lxc/lxc/blob/master/src/lxc/lxccontainer.h
 */
void LxcContainerType::createContainer(std::shared_ptr<ns3lxc::Node> nodePtr) {
    lxc_container *c;
    containerMap[nodePtr->name] = shared_ptr<lxc_container>(lxc_container_new(nodePtr->name.c_str(), NULL));
    c = containerMap[nodePtr->name].get();
    if(!c){
        cerr << "PROBLEMS WITH CONTAINER" << endl;
    }
    if(c->is_defined(c)){
        //container exists, destroy
        if(!c->shutdown(c, 10)){
            //didn't shutdown
            c->stop(c);
        }
        c->destroy(c);
    }
    if(!c->createl(c, "download", NULL, NULL, LXC_CREATE_QUIET, "-d", containerDistro.c_str(), "-r", containerRelease.c_str(), "-a", "amd64", NULL))
    {
        //create failed
        cerr << "Container " << nodePtr->name << " could not be created" << endl;
    }
}
void LxcContainerType::startContainer(std::shared_ptr<ns3lxc::Node> nodePtr) {
    lxc_container *c = containerMap[nodePtr->name].get();
    if (!c->start(c, 0, NULL)) {
        //failed to start
        cerr << "Container " << nodePtr->name << " failed to start" << endl;
    }
}
void LxcContainerType::installApplications(std::shared_ptr<ns3lxc::Node> nodePtr) {

}
void LxcContainerType::runApplications(std::shared_ptr<ns3lxc::Node> nodePtr) {

}
void LxcContainerType::grabOutput(std::shared_ptr<ns3lxc::Node> nodePtr) {

}

void LxcContainerType::teardownContainer(std::shared_ptr<ns3lxc::Node> nodePtr){
    lxc_container *c = containerMap[nodePtr->name].get();
    if(c->is_defined(c)){
        //container exists, destroy
        if(!c->shutdown(c, 10)){
            //didn't shutdown
            c->stop(c);
        }
        c->destroy(c);
    }
    c = nullptr;
    containerMap.erase(nodePtr->name);

}