#ifndef INETADDRESS_H
#define INETADDRESS_H

#include <cctype>
#include <cassert>
#include <cstring>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace Tube
{
    class InetAddress
    {
    public:
        InetAddress();

        InetAddress(
            const InetAddress& other);

        InetAddress(
            const struct sockaddr* address,
            socklen_t              addressLength);

        InetAddress&
        operator=(
            const InetAddress& other);

        ~InetAddress();

        bool
        operator==(
            const InetAddress& other) const;

        bool
        init(
            const std::string& ipStr);

        bool
        init(
            const char* ipStr);

        bool
        init(
            const struct sockaddr_storage* address,
            socklen_t                      addressLength);

        void
        set_port(
            unsigned short port);

        const struct sockaddr*
        get_address() const;

        socklen_t
        get_address_length() const;

        void
        get_socket_address(
            int&              inetFamily,
            sockaddr_storage& address,
            unsigned short&   port,
            socklen_t&        addressLength) const;

    private:
        void
        skip_spaces(
            const char*& str);

    private:
        union Address
        {
            struct sockaddr_in  addrIn4;
            struct sockaddr_in6 addrIn6;
            struct sockaddr     addrRaw;
        }addressM;
    };

    inline
    InetAddress::InetAddress()
    {
        memset(&addressM, 0, sizeof(addressM));
    }

    inline
    InetAddress::InetAddress(
        const InetAddress& other)
    : addressM(other.addressM)
    {
        // Empty
    }

    inline
    InetAddress::InetAddress(
        const struct sockaddr* address,
        socklen_t              addressLength)
    {
        assert(addressLength <= sizeof(addressM));
        memcpy(&addressM, address, addressLength);
    }

    inline
    InetAddress&
    InetAddress::operator=(
        const InetAddress& other)
    {
        addressM = other.addressM;
        return *this;
    }

    inline  
    InetAddress::~InetAddress()
    {
        // Empty
    }

    inline
    bool
    InetAddress::operator==(
        const InetAddress& other) const
    {
        return (addressM.addrRaw.sa_family == AF_INET) ?
                memcmp(&addressM.addrIn4,
                       &other.addressM.addrIn4,
                       sizeof(addressM.addrIn4)) == 0 :
                memcmp(&addressM.addrIn6,
                       &other.addressM.addrIn6,
                       sizeof(addressM.addrIn6)) == 0;
    }

    inline
    bool
    InetAddress::init(
        const std::string& ipStr)
    {
        return init(ipStr.c_str());
    }

    inline
    void
    InetAddress::set_port(
            unsigned short port)
    {
        (addressM.addrRaw.sa_family == AF_INET) ?
        addressM.addrIn4.sin_port = htons(port) :
        addressM.addrIn6.sin6_port = htons(port);       
    }

    inline
    const struct sockaddr*
    InetAddress::get_address() const
    {
        return &addressM.addrRaw;
    }

    inline
    socklen_t
    InetAddress::get_address_length() const
    {
        return (addressM.addrRaw.sa_family == AF_INET) ?
               sizeof(struct sockaddr_in) :
               sizeof(struct sockaddr_in6);
    }

    inline
    void
    InetAddress::get_socket_address(
        int&              inetFamily,
        sockaddr_storage& address,
        unsigned short&   port,
        socklen_t&        addressLength) const
    {
        inetFamily = addressM.addrRaw.sa_family;
        if (inetFamily == AF_INET)
        {
            addressLength = sizeof(addressM.addrIn4);
            memcpy(&address, &addressM.addrIn4, addressLength);
            port = ntohs(((struct sockaddr_in*)&address)->sin_port);
        }
        else
        {
            addressLength = sizeof(addressM.addrIn6);
            memcpy(&address, &addressM.addrIn6, addressLength);
            port = ntohs(((struct sockaddr_in6*)&address)->sin6_port);
        }
    }


    inline
    void
    InetAddress::skip_spaces(
        const char*& str)
    {
        while (isspace(*str))
        {
            str++;
        }
    }
}

#endif