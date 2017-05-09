#include <string>
#include <iterator>
#include <cstdint>

#include <arpa/inet.h>

#include "ipaddr.h"

using namespace ns3lxc;

IpAddr::IpAddr(const IpAddr& temp){
    ipv4 = temp.ipv4;
    if(ipv4){
        ipv4_address = temp.ipv4_address;
    } else {
        std::copy(temp.ipv6_address_8, temp.ipv6_address_8 + INET6_ADDRSTRLEN, ipv6_address_8);
    }
}

IpAddr::IpAddr(uint32_t ipv4_addr){
    ipv4_address = ipv4_addr;
    ipv4 = true;
}

IpAddr::IpAddr(uint8_t *ipv6){
    std::copy(ipv6, ipv6 + INET6_ADDRSTRLEN, ipv6_address_8);
    ipv4 = false;
}

IpAddr::IpAddr(int af, std::string addr){
    switch(af){
        default:
        case(AF_INET):
            inet_pton(af, addr.c_str(), (void *) &ipv4_address);
            ipv4_address = ntohl(ipv4_address);
            ipv4 = true;
            break;
        case(AF_INET6):
            inet_pton(af, addr.c_str(), ipv6_address_32);
            ipv6_address_32[0] = ntohl(ipv6_address_32[0]);
            ipv6_address_32[1] = ntohl(ipv6_address_32[1]);
            ipv6_address_32[2] = ntohl(ipv6_address_32[2]);
            ipv6_address_32[3] = ntohl(ipv6_address_32[3]);
            ipv4 = false;
            break;
    }
}

std::string IpAddr::str(){
    char buf[INET6_ADDRSTRLEN];
    uint32_t temp;
    if(ipv4) {
        temp = htonl(ipv4_address);
        inet_ntop(AF_INET, &(temp), buf, INET_ADDRSTRLEN);
    } else {
        //Probably backwards
        inet_ntop(AF_INET6, ipv6_address_8, buf, INET6_ADDRSTRLEN);
    }

    return std::string(buf);
}

void IpAddr::applyOffset(std::string offset){
    uint32_t temp;
    if(ipv4) {
        inet_pton(AF_INET, offset.c_str(), &(temp));
        temp = ntohl(temp);
        ipv4_address = ipv4_address + temp;
    } else {
        //Probably backwards
        // inet_ntop(AF_INET6, ipv6_address_8, buf, INET6_ADDRSTRLEN);
    }
}