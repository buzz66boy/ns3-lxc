#ifndef __APPLICATION_TYPE_H_INCLUDED__
#define __APPLICATION_TYPE_H_INCLUDED__

#include <map>
#include <string>
#include <utility>
#include <sstream>
#include <ostream>
#include <memory>
#include <vector>

#include "application.h"
#include "node.h"
#include "topology.h"

/*
packman
    configuration files
        already created:
            location local
            location to place
        not created (dependent on node/link params):
            filename to write
            location to place file
    execution commands
    files to retrieve
binary/script/folder
    location of
    location to place
 */
enum class InstallMethod {PACKMAN, BIN, FOLDER, SCRIPT};

class ApplicationType {
public:
    /**
     * Denotes whether the application should start with the simulation or before
     *
     **/
    virtual bool isApplicationSynced() = 0;

    /**
     * Returns what type of install method the application uses (see InstallMethod enum above)
     **/
    virtual InstallMethod getInstallMethod() = 0;

    /**
     * Return string pair,
     * first is local location of program (binary, script, folder, etc)
     * second is location on node. If first is filled but second empty, location is assumed same
     **/
    virtual std::pair<std::string, std::string> getLocation(ns3lxc::Application *app) = 0;

    /**
     * Returns a list of string pairs
     * first in pair is local file location (path)
     * second is file location to copy file/folder to on node
     **/
    virtual std::vector<std::pair<std::string, std::string> > getExistingRequiredFiles(ns3lxc::Application *app, std::shared_ptr<ns3lxc::Node> nodePtr) = 0;

    /**
     * Returns a list of string, stringstream pairs
     * First string is location to place file on node
     * Second stringstream are the contents to write to that file
     **/
    virtual std::vector<std::pair<std::string, std::string> > getRequiredFiles(ns3lxc::Application *app, std::shared_ptr<ns3lxc::Node> nodePtr) = 0;

    /**
     * Returns a list of strings to run sequentially to execute program on the node
     * boolean tells whether the executing script should wait for the command to complete before running the next command
     **/
    virtual std::vector<std::pair<std::string, bool> > getExecutionCommands(ns3lxc::Application *app, std::shared_ptr<ns3lxc::Node> nodePtr) = 0;

    /**
     * Returns a list of locations to grab files from the node (like log files)
     **/
    virtual std::vector<std::string> getCleanupLocations(ns3lxc::Application *app, std::shared_ptr<ns3lxc::Node> nodePtr) = 0;

    /**
     * Returns a list of commands to be run sequentially on a node to run any cleanup actions (usually actions effecting the host)
     **/
    virtual std::vector<std::string> getCleanupCommands(ns3lxc::Application *app, std::shared_ptr<ns3lxc::Node> nodePtr) = 0;
};

#endif