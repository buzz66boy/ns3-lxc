#ifndef __NODE_TYPE_H_INCLUDED__
#define __NODE_TYPE_H_INCLUDED__

#include <map>
#include <string>
#include <ostream>
#include <memory>

#include "node.h"

class NodeType {
public:
    /**
     * Returns if bridges and taps should be assigned for the node in question
     **/
    virtual bool createBridgesTaps() = 0;

    /**
     * Create the node
     **/
    virtual void createNode(std::shared_ptr<yntdl::Node>) = 0;

    /**
     * Start the node
     **/
    virtual void startNode(std::shared_ptr<yntdl::Node>) = 0; //Change to string?

    /**
     * Make necessary type-specific steps to install the applications on any container
     *  such as pre-downloading packages
     **/
    virtual void prepForInstall(std::vector<std::shared_ptr<yntdl::Application> >) = 0;

    /**
     * Install the applications in node->applications on the node and run them
     * if the application type is not synched
     **/
    virtual void installApplications(std::shared_ptr<yntdl::Node>) = 0;

    /**
     * Run the commands specified for node that aren't tied to an application
     **/
    virtual void runCommands(std::shared_ptr<yntdl::Node>) = 0;

    /**
     * Start the applications on the node if they are synched and run the commands associated with them
     **/
    virtual void runApplications(std::shared_ptr<yntdl::Node>) = 0; // for now, change to sync'ed packet later?
    
    /**
     * Grab any output (such as log files) from the apps running on the node
     **/
    virtual void grabOutput(std::shared_ptr<yntdl::Node>) = 0;

    /**
     * Shutdown/destroy the node
     **/
    virtual void teardownNode(std::shared_ptr<yntdl::Node>) = 0;
};

#endif