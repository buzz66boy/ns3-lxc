#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include <sys/socket.h>

#include "yaml-cpp/yaml.h"

#include "parserTags.h"
#include "errorCode.h"
#include "ifaceParser.h"

using namespace std;

std::weak_ptr<yntdl::IfaceProvider> getProvider(string provider, yntdl::Topology *top){
    if(top->nodeMap.count(provider) > 0){
        return top->nodeMap[provider];
    } else if(top->topMap.count(provider) > 0){
        return top->topMap[provider];
    } else {
        throw Ns3lxcException(ErrorCode::PROVIDER_NOT_FOUND, provider);
    }
}

std::weak_ptr<yntdl::IfaceAcceptor> getAcceptor(string acceptor, yntdl::Topology *top){
        if(top->topMap.count(acceptor) > 0){
            return top->topMap[acceptor];
        } else if(top->linkMap.count(acceptor) > 0){
            return top->linkMap[acceptor];
        } else {
            throw Ns3lxcException(ErrorCode::ACCEPTOR_NOT_FOUND, acceptor);
        }
}

void parseIfacesProvided(YAML::Node ifaces, ParsedTopology *parsedTop){
    for(auto i = 0; i < ifaces.size(); ++i){
        vector<string> split = splitString(ifaces[i].begin()->second.as<string>());
        parsedTop->topology.ifacesProvidedSubNames[ifaces[i].begin()->first.as<string>()] = split[1];
        std::weak_ptr<yntdl::IfaceProvider> provPtr = getProvider(split[0], &parsedTop->topology);
        parsedTop->topology.ifacesProvided[ifaces[i].begin()->first.as<string>()] = provPtr;
    }
}

void parseAcceptedIfaces(YAML::Node acceptedIface, ParsedTopology *parsedTop){
    for(auto i = 0; i < acceptedIface.size(); ++i){
        string acceptor = acceptedIface[i].begin()->first.as<string>();
        string provider = acceptedIface[i].begin()->second.as<string>();
        cout << "Connecting " + provider + " to " + acceptor << endl;
        vector<string> acceptSplit = splitString(acceptor);
        vector<string> provideSplit = splitString(provider);

        //Find Provider
        shared_ptr<yntdl::IfaceProvider> provPtr = getProvider(provideSplit[0], &parsedTop->topology).lock();
        shared_ptr<yntdl::IfaceAcceptor> accPtr = getAcceptor(acceptSplit[0], &parsedTop->topology).lock();

        yntdl::Iface *iface = provPtr->getIface(provideSplit[1]);
        if(provideSplit.size() < 3){
            throw Ns3lxcException(ErrorCode::NO_IP, provider);
        } else {
            iface->assignIp(AF_INET, provideSplit[2]);   
        }
        if(acceptSplit.size() > 1){
            accPtr->connectIface(acceptSplit[1], provPtr->getIface(provideSplit[1]));
        } else {
            //assume it's a local link
            accPtr->connectIface("", provPtr->getIface(provideSplit[1]));
        }
    }
}

void parseIfacesAccepted(YAML::Node ifacesAccepted, ParsedTopology *parsedTop){
    for(auto i = 0; i < ifacesAccepted.size(); ++i){
        vector<string> split = splitString(ifacesAccepted[i].begin()->second.as<string>());
        if(split.size() > 1){
            parsedTop->topology.ifacesAcceptedSubNames[ifacesAccepted[i].begin()->first.as<string>()] = split[1];
        } else {
            parsedTop->topology.ifacesAcceptedSubNames[ifacesAccepted[i].begin()->first.as<string>()] = "";
        }
        parsedTop->topology.ifacesAccepted[ifacesAccepted[i].begin()->first.as<string>()] = getAcceptor(split[0], &parsedTop->topology);
    }
}