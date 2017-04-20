#include <ostream>

#include "wifi.h"

using namespace std;

int Wifi::getIfacesSupported(){
    return 2;
}
void Wifi::writeIncludes(std::ostream str){
    //str << "#include \"ns3/csma-module.h\"" << endl;
}
void Wifi::writeTypeInit(std::ostream str){
    //str << "CsmaHelper csma;" << endl;
}
void Wifi::writeLinkInit(std::ostream str, std::string linkName, int linkNum){

}
void Wifi::addIfaceToLink(std::ostream str, std::string ifaceName, std::string linkName, int linkNum){

}