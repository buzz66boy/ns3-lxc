#include <ostream>

#include "csma.h"

using namespace std;

int CSMA::getIfacesSupported(){
    return 2;
}
void CSMA::writeIncludes(std::ostream str){
    str << "#include \"ns3/csma-module.h\"" << endl;
}
void CSMA::writeTypeInit(std::ostream str){
    str << "CsmaHelper csma;" << endl;
}
void CSMA::writeLinkInit(std::ostream str, std::string linkName, int linkNum){

}
void CSMA::addIfaceToLink(std::ostream str, std::string ifaceName, std::string linkName, int linkNum){

}