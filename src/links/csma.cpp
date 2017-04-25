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

int CSMA::getIfacesSupported(){
    return 2;
}
void CSMA::writeIncludes(std::ostream& str){
    str << "#include \"ns3/csma-module.h\"" << endl;
}
void CSMA::writeTypeInit(std::ostream& str){
    str << "CsmaHelper csma;" << endl;
    //FIXME
    str << "csma.SetChannelAttribute(\"DataRate\",StringValue(\"10Mbps\"));" << endl;
    str << "TapBridgeHelper csmaTapBridge;" << endl;
    str << "csmaTapBridge.SetAttribute (\"Mode\", StringValue (\"UseLocal\"));" << endl;
    str << "csma.EnablePcapAll(\"csma_pcap\",true);" << endl;
}
void CSMA::writeLinkInit(std::ostream& str, shared_ptr<ns3lxc::Link> linkPtr){
    string contName = linkPtr->name + "_container";
    string devName = linkPtr->name + "_dev";
    str << "NodeContainer " + contName + " = NodeContainer(";

    vector<shared_ptr<ns3lxc::Iface> > ifaces = linkPtr->getIfaces();
    for(auto i = 0; i < ifaces.size(); ++i){
        str << "nodes.Get(" + to_string(ifaces[i]->node->nodeNum) + ")";
        if(i != ifaces.size() - 1){
            str << ", ";
        }
    }
    str << ");" << endl;

    str << "NetDeviceContainer " + devName + " = csma.Install(" + contName + ");" << endl;

    for(auto i = 0; i < ifaces.size(); ++i){
        str << "csmaTapBridge.SetAttribute (\"DeviceName\", StringValue (\"" + ifaces[i]->tapName + "\"));" << endl;
        str << "csmaTapBridge.Install(nodes.Get(" + to_string(ifaces[i]->node->nodeNum) + "), " + devName + ".Get(" + to_string(i) + "));" << endl;
    }

}
void CSMA::addIfacesToLink(std::ostream& str, shared_ptr<ns3lxc::Link> linkPtr){

}