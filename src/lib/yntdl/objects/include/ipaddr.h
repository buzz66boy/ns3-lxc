#ifndef __IPADDR_H_INCLUDED__
#define __IPADDR_H_INCLUDED__

// include dependencies
#include <string>
#include <vector>
#include <map>
#include <cstdint>

#include <arpa/inet.h>

namespace yntdl {

class IpAddr {
private:
    bool ipv4;
    union {
        uint8_t ipv6_address_8[16];
        uint32_t ipv6_address_32[4];
        uint32_t ipv4_address;
    };

public:
    IpAddr(const IpAddr&);
    IpAddr(uint32_t ipv4_addr);
    
    IpAddr(uint8_t *ipv6_addr);
    IpAddr(uint32_t *ipv6_addr) { IpAddr((uint8_t *)ipv6_addr); }

    IpAddr(int af, std::string addr);
    IpAddr(int af, int cidr);
    ~IpAddr() {}

    bool equals(IpAddr &temp);

    bool isIpv4() { return ipv4; }
    uint32_t getIpv4Addr() { return ipv4_address; }
    std::vector<uint32_t> getIpv6Addr();

	std::string str();
    int getCidr();
    void applyOffset(std::string);
    std::string getSubnet(IpAddr *subnetMask);
};

}

#endif