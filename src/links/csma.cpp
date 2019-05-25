#include <ostream>
#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include "link.h"
#include "iface.h"
#include "node.h"
#include "csma.h"

using namespace std;

static std::string defaultDataRate = "10Mbps";
static std::string defaultLatency = "2"; //ms

int CSMA::getIfacesSupported(){
    return 2;
}
void CSMA::writeIncludes(std::ostream& str){
    str << "#include \"ns3/csma-module.h\"" << endl;
}
void CSMA::writeTypeInit(std::ostream& str){
    str << "CsmaHelper csma;" << endl;
    //FIXME
    str << "csma.SetChannelAttribute(\"DataRate\",StringValue(\"" + defaultDataRate + "\"));" << endl;
    str << "TapBridgeHelper csmaTapBridge;" << endl;
    str << "csmaTapBridge.SetAttribute (\"Mode\", StringValue (\"UseLocal\"));" << endl;
}
void CSMA::writeLinkInit(std::ostream& str, shared_ptr<yntdl::Link> linkPtr){
    string contName = linkPtr->name + "_container";
    string devName = linkPtr->name + "_dev";
    str << "NodeContainer " + contName + " = NodeContainer(";

    for(auto i = 0; i < linkPtr->ifaces.size(); ++i){
        str << "nodes.Get(" + to_string(linkPtr->ifaces[i]->node->nodeNum) + ")";
        if(i != linkPtr->ifaces.size() - 1){
            str << ", ";
        }
    }
    str << ");" << endl;

    if(linkPtr->bandwidth != ""){
        str << "csma.SetChannelAttribute(\"DataRate\", StringValue(\"" + linkPtr->bandwidth + "\"));" << endl;
    } else {
        str << "csma.SetChannelAttribute(\"DataRate\", StringValue(\"" + defaultDataRate + "\"));" << endl;
    }
    if(linkPtr->latency != ""){
        str << "csma.SetChannelAttribute(\"Delay\", TimeValue(MilliSeconds(" + linkPtr->latency + ")));" << endl;                
    } else {
        str << "csma.SetChannelAttribute(\"Delay\", TimeValue(MilliSeconds(" + defaultLatency + ")));" << endl;
    }

    str << "NetDeviceContainer " + devName + " = csma.Install(" + contName + ");" << endl;

    for(auto i = 0; i < linkPtr->ifaces.size(); ++i){
        str << "csmaTapBridge.SetAttribute (\"DeviceName\", StringValue (\"" + linkPtr->ifaces[i]->tapName + "\"));" << endl;
        str << "csmaTapBridge.Install(nodes.Get(" + to_string(linkPtr->ifaces[i]->node->nodeNum) + "), " + devName + ".Get(" + to_string(i) + "));" << endl;
    }

}
void CSMA::addIfacesToLink(std::ostream& str, shared_ptr<yntdl::Link> linkPtr){

}

void CSMA::writeClosingRemarks(std::ostream& str){
    str << "csma.EnablePcapAll(\"csma_pcap\",true);" << endl;
}