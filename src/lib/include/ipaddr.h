#ifndef __IPADDR_H_INCLUDED__
#define __IPADDR_H_INCLUDED__

// include dependencies
#include <string>
#include <vector>
#include <map>

#include <arpa/inet.h>

class Ipaddr {
private:
    bool ipv4;
    union {
        uint8_t ipv6_address_8[16];
        uint32_t ipv6_address_32[4];
        uint32_t ipv4_address;
    };
public:
    Ipaddr(uint32_t ipv4_addr);
    
    Ipaddr(uint8_t *ipv6_addr);
    Ipaddr(uint32_t *ipv6_addr) { Ipaddr((uint8_t *)ipv6_addr); }

    Ipaddr(int af, std::string addr);

    bool is_ipv4() { return ipv4; }

	std::string str();
};

#endif