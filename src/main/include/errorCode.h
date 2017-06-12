#ifndef __ERROR_CODE_H_INCLUDED__
#define __ERROR_CODE_H_INCLUDED__

#include <exception>
#include <string>

enum class ErrorCode {
//General Error Code
    NOT_ENOUGH_ARGS,
    NO_FILE_PROVIDED,
    RUN_AS_ROOT,
    FOLDER_NOT_FOUND,
    FOLDER_NOT_CREATED,
    FILE_NOT_FOUND,
    SETTINGS_ATTR_NOT_FOUND,
//Parse Error Codes
    //GENERAL
    TEMPLATE_NOT_FOUND,
    POSITION_ERROR,
    //TOP
    TOPOLOGY_NOT_FOUND,
    //NODES
    NODE_NOT_FOUND,
    NODE_TYPE_NOT_FOUND,
    NODE_TYPE_NOT_SPECIFIED,
    //APPS
    APP_NOT_FOUND,
    NODE_NOT_SPECIFIED,
    //LINKS
    LINK_NOT_FOUND,
    LINK_TYPE_NOT_FOUND,
    LINK_TYPE_NOT_SPECIFIED,
    //IFACES
    IFACE_NOT_FOUND,
    ACCEPTOR_NOT_FOUND,
    PROVIDER_NOT_FOUND,
    NO_IP,
//Validation Error Codes
    TOPOLOGY_INVALID,
    LINK_INVALID,
    NODE_INVALID,
    APP_INVALID,
//Generation Error Codes
    BR_CREATE_FAILURE,
    TAP_CREATE_FAILURE,
    NODE_CREATE_FAILURE,
    NODE_START_FAILURE,
    NODE_TEARDOWN_FAILURE,
    APP_INSTALL_FAILURE,
    APP_RUN_FAILURE,
    NS3_WRITE_FAILURE,
    NS3_RUN_FAILURE,
};

class Ns3lxcException : public std::exception {
private:
    ErrorCode errorCode;
    std::string relevantName;
public:
    Ns3lxcException(ErrorCode ec, std::string relName): errorCode(ec), relevantName(relName) {};
    virtual const char* what() const throw();

    Ns3lxcException(const Ns3lxcException& temp): errorCode(temp.errorCode), relevantName(temp.relevantName) {}
};

#endif