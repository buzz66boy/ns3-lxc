#ifndef __IPADDR_H_INCLUDED__
#define __IPADDR_H_INCLUDED__

// include dependencies
#include <string>
#include <vector>
#include <map>
#include <cstdint>

#include <arpa/inet.h>

namespace ns3lxc {

class IpAddr {
private:
    bool ipv4;
    union {
        uint8_t ipv6_address_8[16];
        uint32_t ipv6_address_32[4];
        uint32_t ipv4_address;
    };

public:
    IpAddr(uint32_t ipv4_addr);
    
    IpAddr(uint8_t *ipv6_addr);
    IpAddr(uint32_t *ipv6_addr) { IpAddr((uint8_t *)ipv6_addr); }

    IpAddr(int af, std::string addr);

    bool is_ipv4() { return ipv4; }

	std::string str();
};

}

#endif