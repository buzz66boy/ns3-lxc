#include <ostream>

#include "wifi.h"

using namespace std;

int WIFI::getIfacesSupported(){
    return 2;
}
void WIFI::writeIncludes(std::ostream str){
    str << "#include \"ns3/csma-module.h\"" << endl;
}
void WIFI::writeTypeInit(std::ostream str){
    str << "CsmaHelper csma;" << endl;
}
void WIFI::writeLinkInit(std::ostream str, std::string linkName, int linkNum){

}
void WIFI::addIfaceToLink(std::ostream str, std::string ifaceName, std::string linkName, int linkNum){

}