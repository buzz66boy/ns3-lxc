#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include <sys/socket.h>

#include "yaml-cpp/yaml.h"

#include "parserTags.h"
#include "ifaceParser.h"

using namespace std;

static std::weak_ptr<ns3lxc::IfaceProvider> getProvider(string provider, ParsedTopology *parsedTop){
    if(parsedTop->topology.nodeMap.count(provider) > 0){
        return parsedTop->topology.nodeMap[provider];
    } else if(parsedTop->topology.topMap.count(provider) > 0){
        return parsedTop->topology.topMap[provider];
    } else {
        cerr << "COULDNT FIND" << endl;
        return weak_ptr<ns3lxc::IfaceProvider>();
    }
}

static std::weak_ptr<ns3lxc::IfaceAcceptor> getAcceptor(string acceptor, ParsedTopology *parsedTop){
        if(parsedTop->topology.topMap.count(acceptor) > 0){
            return parsedTop->topology.topMap[acceptor];
        } else if(parsedTop->topology.linkMap.count(acceptor) > 0){
            return parsedTop->topology.linkMap[acceptor];
        } else {
            cerr << "COULDNT FIND" << endl;
            return weak_ptr<ns3lxc::IfaceAcceptor>();
        }
}

void parseIfacesProvided(YAML::Node ifaces, ParsedTopology *parsedTop){
    for(auto i = 0; i < ifaces.size(); ++i){
        vector<string> split = splitString(ifaces[i].begin()->second.as<string>());
        parsedTop->topology.ifacesProvidedSubNames[ifaces[i].begin()->first.as<string>()] = split[1];
        std::weak_ptr<ns3lxc::IfaceProvider> provPtr = getProvider(split[0], parsedTop);
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
        shared_ptr<ns3lxc::IfaceProvider> provPtr = getProvider(provideSplit[0], parsedTop).lock();
        shared_ptr<ns3lxc::IfaceAcceptor> accPtr = getAcceptor(acceptSplit[0], parsedTop).lock();

        ns3lxc::Iface *iface = provPtr->getIface(provideSplit[1]);
        if(provideSplit.size() < 3){
            cerr << "No IP specified or invalid provider for " + provider << endl;
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
        parsedTop->topology.ifacesAccepted[ifacesAccepted[i].begin()->first.as<string>()] = getAcceptor(split[0], parsedTop);
    }
}