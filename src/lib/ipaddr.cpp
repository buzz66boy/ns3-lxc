#include <string>
#include <iterator>

#include <arpa/inet.h>

#include "ipaddr.h"

using namespace std;

Ipaddr::Ipaddr(uint32_t ipv4){
    ipv4_address = ipv4;
    ipv4 = true;
}

Ipaddr::Ipaddr(uint8_t *ipv6){
    std::copy(ipv6, ipv6 + 16, ipv6_address_8);
    ipv4 = false;
}

Ipaddr::Ipaddr(int af, std::string addr){
    switch(af){
        case(AF_INET):
            inet_pton(af, addr.c_str(), (void *) &ipv4_address);
            ipv4_address = ntohl(ipv4_address);
            break;
        case(AF_INET6):
            inet_pton(af, addr.c_str(), ipv6_address_32);
            ipv6_address_32[0] = ntohl(ipv6_address_32[0]);
            ipv6_address_32[1] = ntohl(ipv6_address_32[1]);
            ipv6_address_32[2] = ntohl(ipv6_address_32[2]);
            ipv6_address_32[3] = ntohl(ipv6_address_32[3]);
            break;
    }
}

std::string Ipaddr::str(){
    char buf[INET6_ADDRSTRLEN];

    if(ipv4)
    {
        inet_ntop(AF_INET, &(ipv4_address), buf, INET_ADDRSTRLEN);
    } else {
        inet_ntop(AF_INET6, ipv6_address_8, buf, INET6_ADDRSTRLEN);
    }

    return std::string(buf);
}