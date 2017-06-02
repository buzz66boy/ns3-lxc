#include <string>
#include <cstring>

#include "errorCode.h"

using namespace std;

const char* Ns3lxcException::what() const throw() {
    string errStr = "";
    switch(errorCode){
        case(ErrorCode::NOT_ENOUGH_ARGS):
            errStr = "Not enough arguments: ";
            break;
        case(ErrorCode::NO_FILE_PROVIDED):
            errStr = "No file provided";
            break;
        case(ErrorCode::RUN_AS_ROOT):
            errStr = "Must run with root privileges (sudo)";
            break;
        case(ErrorCode::FOLDER_NOT_FOUND):
            errStr = "Folder not found: ";
            break;
        case(ErrorCode::FOLDER_NOT_CREATED):
            errStr = "Folder not created: ";
            break;
        case(ErrorCode::FILE_NOT_FOUND):
            errStr = "File not found: ";
            break;
        case(ErrorCode::SETTINGS_ATTR_NOT_FOUND):
            errStr = "Settings attribute not found/set: ";
            break;
        //Parse Error Codes
        //GENERAL
        case(ErrorCode::TEMPLATE_NOT_FOUND):
            errStr = "Template not found: ";
            break;
        case(ErrorCode::POSITION_ERROR):
            errStr = "Error in Positions: ";
            break;
        //TOP
        case(ErrorCode::TOPOLOGY_NOT_FOUND):
            errStr = "Topology not found: ";
            break;
        //NODES
        case(ErrorCode::NODE_NOT_FOUND):
            errStr = "Node not found: ";
            break;
        case(ErrorCode::NODE_TYPE_NOT_FOUND):
            errStr = "Node Type not Mapped: ";
            break;
        case(ErrorCode::NODE_TYPE_NOT_SPECIFIED):
            errStr = "Node Type not specified: ";
            break;
        //LINKS
        case(ErrorCode::LINK_NOT_FOUND):
            errStr = "Link not found: ";
            break;
        case(ErrorCode::LINK_TYPE_NOT_FOUND):
            errStr = "Link Type not mapped: ";
            break;
        case(ErrorCode::LINK_TYPE_NOT_SPECIFIED):
            errStr = "Link Type no specified";
            break;
        //IFACES
        case(ErrorCode::IFACE_NOT_FOUND):
            errStr = "Iface not found: ";
            break;
        case(ErrorCode::ACCEPTOR_NOT_FOUND):
            errStr = "Iface Acceptor not found: ";
            break;
        case(ErrorCode::PROVIDER_NOT_FOUND):
            errStr = "Iface Provider not found: ";
            break;
        case(ErrorCode::NO_IP):
            errStr = "No IP: ";
            break;
        //Validation Error Codes
        case(ErrorCode::TOPOLOGY_INVALID):
            errStr = "Invalid Topology: ";
            break;
        case(ErrorCode::LINK_INVALID):
            errStr = "Invalid Link: ";
            break;
        case(ErrorCode::NODE_INVALID):
            errStr = "Invalid Node: ";
            break;
        case(ErrorCode::APP_INVALID):
            errStr = "Invalid Application: ";
            break;
        //Generation Error Codes
        case(ErrorCode::BR_CREATE_FAILURE):
            errStr = "Failed to create bridge: ";
            break;
        case(ErrorCode::TAP_CREATE_FAILURE):
            errStr = "Failed to create tap: ";
            break;
        case(ErrorCode::NODE_CREATE_FAILURE):
            errStr = "Failed to create Node: ";
            break;
        case(ErrorCode::NODE_START_FAILURE):
            errStr = "Failed to start Node: ";
            break;
        case(ErrorCode::NODE_TEARDOWN_FAILURE):
            errStr = "Failed to teardown Node: ";
            break;
        case(ErrorCode::APP_INSTALL_FAILURE):
            errStr = "Failed to install Application: ";
            break;
        case(ErrorCode::APP_RUN_FAILURE):
            errStr = "Failed to run Application: ";
            break;
        case(ErrorCode::NS3_WRITE_FAILURE):
            errStr = "Error in writing ns-3 script: ";
            break;
        case(ErrorCode::NS3_RUN_FAILURE):
            errStr = "Failed to run ns-3: ";
            break;
        default:
            errStr = "Error: ";
            break;
    }
    string err = errStr + relevantName;
    const char *buf = (const char *) malloc(err.length() * sizeof(char));
    strcpy((char *)buf, err.c_str());
    return buf;
}