#include <ostream>
#include <iostream>
#include <memory>

#include "wifi.h"

using namespace std;

int Wifi::getIfacesSupported(){
    return 2;
}
void Wifi::writeIncludes(std::ostream& str){
    str << "#include \"ns3/wifi-module.h\"" << endl;
}
void Wifi::writeTypeInit(std::ostream& str){
    str << "WifiHelper wifi;" << endl;
    str << "wifi.SetStandard (WIFI_PHY_STANDARD_80211a);" << endl;
    str << "YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();" << endl;
    str << "NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();" << endl;
    str << "wifiMac.SetType (\"ns3::AdhocWifiMac\");" << endl;
    str << "Config::SetDefault(\"ns3::WifiMacQueue::MaxPacketNumber\", UintegerValue(10000));" << endl;
    str << "Config::SetDefault(\"ns3::WifiMacQueue::MaxDelay\", TimeValue(Seconds(600)));" << endl;
    str << "Config::SetDefault (\"ns3::WifiRemoteStationManager::RtsCtsThreshold\", StringValue (\"2200\"));" << endl;
    str << "Config::SetDefault (\"ns3::WifiRemoteStationManager::FragmentationThreshold\", StringValue (\"2200\"));" << endl;

}
void Wifi::writeLinkInit(std::ostream& str, shared_ptr<ns3lxc::Link> linkPtr){

}
void Wifi::addIfacesToLink(std::ostream& str, shared_ptr<ns3lxc::Link> linkPtr){

}