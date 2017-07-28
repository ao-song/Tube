#include "InetAddress.h"

using namespace Tube;

// ----------------------------------------------------------------------------

bool
InetAddress::init(
    const char* ipStr)
{
    skip_spaces(ipStr);

    const char* startOfAddress = ipStr;

    enum {None, IPv4, IPv6} ipVersion = None;

    while(true)
    {
        switch(ipVersion)
        {
            case None:
            {               
                if(*ipStr == '.')
                {
                    ipVersion = IPv4;
                }
                else if(isxdigit(*ipStr) || *ipStr == ':')
                {
                    ipVersion = IPv6
                }
                else if(!isdigit(*ipStr))
                {
                    // what is this then?
                    return false;
                }
                break;
            }
            case IPv4:
            {
                if(!(isdigit(*ipStr) || (*ipStr == '.')))
                {
                    size_t addressLength = ipStr - startOfAddress;
                    if(addressLength > 15)
                    {
                        // not an IP address
                        return false;
                    }

                    unsigned char buf[16];
                    memcpy(buf, startOfAddress, addressLength);
                    buf[addressLength] = 0;

                    skip_spaces(ipStr);
                    if(*ipStr != 0)
                    {
                        return false;
                    }

                    memset(&addressM.addrIn4, 0, sizeof(struct sockaddr_in));
                    addressM.addrIn4.sin_family = AF_INET;
                    return inet_pton(
                        AF_INET, buf, &addressM.addrIn4.sin_addr) == 1;
                }
                break;
            }
            case IPv6:
            {
                if(!(isxdigit(*ipStr) || (*ipStr == ':')))
                {
                    size_t addressLength = ipStr - startOfAddress;
                    if(addressLength > 39)
                    {
                        return false;
                    }

                    unsigned char buf[40];
                    memcpy(buf, startOfAddress, addressLength);
                    buf[addressLength] = 0;

                    skip_spaces(ipStr);
                    if(*ipStr != 0)
                    {
                        return false;
                    }

                    memset(&addressM.addrIn6, 0, sizeof(struct sockaddr_in6));
                    addressM.addrIn6.sin6_family = AF_INET6;
                    return inet_pton(
                        AF_INET6, buf, &addressM.addrIn6.sin6_addr) == 1;
                }
                break;
            }
            default:
            {
                return false;
            }
        }

        ipStr++;
    }
}

// ----------------------------------------------------------------------------

bool
InetAddress::init(
    const struct sockaddr_storage* address,
    socklen_t                      addressLength)
{
    if(reinterpret_cast<const struct sockaddr*>(address)->sa_family == AF_INET)
    {
        // IPv4
        const struct sockaddr_in* addrV4 = 
            reinterpret_cast<const struct sockaddr_in*>(address);
        if(addressLength <= sizeof(addressM))
        {
            memcpy(&addressM, addrV4, addressLength);
        }
        else
        {
            return false;
        }
    }
    else
    {
        // IPv6
        const struct sockaddr_in6* addrV6 = 
            reinterpret_cast<const struct sockaddr_in6*>(address);
        if(addressLength <= sizeof(addressM))
        {
            memcpy(&addressM, addrV6, addressLength);
        }
        else
        {
            return false;
        }
    }

    return true;
}