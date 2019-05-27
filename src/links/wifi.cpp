#include <ostream>
#include <iostream>
#include <memory>
#include <string>

#include "yntdl.h"
#include "wifi.h"
#include "errorCode.h"

using namespace std;

const std::map<std::string, std::string> ofdmRateMap = {
    { "6Mbps", "OfdmRate6Mbps"},
    { "9Mbps", "OfdmRate9Mbps"},
    {"12Mbps", "OfdmRate12Mbps"},
    {"18Mbps", "OfdmRate18Mbps"},
    {"24Mbps", "OfdmRate24Mbps"},
    {"36Mbps", "OfdmRate36Mbps"},
    {"48Mbps", "OfdmRate48Mbps"},
    {"54Mbps", "OfdmRate54Mbps"}
};

static std::string defaultDataRate = "24Mbps";

int Wifi::getIfacesSupported(){
    return 5;
}
void Wifi::writeIncludes(std::ostream& str){
    str << "#include \"ns3/wifi-module.h\"" << endl;
}
void Wifi::writeTypeInit(std::ostream& str){
    str << "WifiHelper wifi;" << endl;
    str << "wifi.SetStandard (WIFI_PHY_STANDARD_80211a);" << endl;
    str << "TapBridgeHelper wifiTapBridge;" << endl;
    str << "wifiTapBridge.SetAttribute (\"Mode\", StringValue (\"UseLocal\"));" << endl;
    str << "YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();" << endl;
    str << "NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();" << endl;
    str << "wifiMac.SetType (\"ns3::AdhocWifiMac\");" << endl;
    str << "Config::SetDefault(\"ns3::WifiMacQueue::MaxPacketNumber\", UintegerValue(10000));" << endl;
    str << "Config::SetDefault(\"ns3::WifiMacQueue::MaxDelay\", TimeValue(Seconds(600)));" << endl;
    str << "Config::SetDefault (\"ns3::WifiRemoteStationManager::RtsCtsThreshold\", StringValue (\"2200\"));" << endl;
    str << "Config::SetDefault (\"ns3::WifiRemoteStationManager::FragmentationThreshold\", StringValue (\"2200\"));" << endl;
    str << "wifi.SetRemoteStationManager (\"ns3::ConstantRateWifiManager\", \"DataMode\", StringValue (\"OfdmRate24Mbps\"));" << endl;
}
void Wifi::writeLinkInit(std::ostream& str, shared_ptr<yntdl::Link> linkPtr){
    string contName = linkPtr->name + "_container";
    string devName = linkPtr->name + "_dev";
    string phyName = linkPtr->name + "_phy";

    if(linkPtr->bandwidth != ""){
        if(ofdmRateMap.count(linkPtr->bandwidth) < 1){
            throw Ns3lxcException(ErrorCode::NS3_WRITE_FAILURE , "Wi-Fi Bandwidth " + linkPtr->bandwidth + " is not a valid ofdm rate");
        } else {
            str << "wifi.SetRemoteStationManager (\"ns3::ConstantRateWifiManager\", \"DataMode\", StringValue (\"" + ofdmRateMap.at(linkPtr->bandwidth) + "\"));" << endl;
        }
    } else {
        str << "wifi.SetRemoteStationManager (\"ns3::ConstantRateWifiManager\", \"DataMode\", StringValue (\"" + ofdmRateMap.at(defaultDataRate) + "\"));" << endl;
    }
    str << "YansWifiPhyHelper " + phyName + " = YansWifiPhyHelper::Default ();" << endl;
    str << phyName + ".SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO);" << endl;
    str << phyName + ".SetChannel (wifiChannel.Create ());" << endl;
    str << "NodeContainer " + contName + " = NodeContainer(";

    for(auto i = 0; i < linkPtr->ifaces.size(); ++i){
        str << "nodes.Get(" + to_string(linkPtr->ifaces[i]->node->nodeNum) + ")";
        if(i != linkPtr->ifaces.size() - 1){
            str << ", ";
        }
    }
    str << ");" << endl;

    str << "NetDeviceContainer " + devName + " = wifi.Install( " + phyName + ", wifiMac, " + contName + ");" << endl;

    for(auto i = 0; i < linkPtr->ifaces.size(); ++i){
        str << "wifiTapBridge.SetAttribute (\"DeviceName\", StringValue (\"" + linkPtr->ifaces[i]->tapName + "\"));" << endl;
        str << "wifiTapBridge.Install(nodes.Get(" + to_string(linkPtr->ifaces[i]->node->nodeNum) + "), " + devName + ".Get(" + to_string(i) + "));" << endl;
    }
    str << phyName + ".EnablePcapAll(\"wifi_" + linkPtr->name + "\",true);" << endl;
    
}

void Wifi::addIfacesToLink(std::ostream& str, shared_ptr<yntdl::Link> linkPtr){

}