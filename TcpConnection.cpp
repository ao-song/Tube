#include "TcpConnection.h"

using namespace Tube;

// ----------------------------------------------------------------------------

TcpConnection::TcpConnection(
    EventHandlerTable*  eventHandlerTable,
    TcpConnectionOwner* tcpConnectionOwner)
:   EventHandler(eventHandlerTable, tcpConnectionOwner),
    stateM(Idle)
{
    // Empty
}

TcpConnection::~TcpConnection()
{
    // Empty
}

bool
TcpConnection::accept(
   int socket)
{
    assert(stateM == Idle);
    set_socket(socket);
 
    if(!make_non_blocking(get_socket()))
    {
        return false;
    }

    stateM = Established;
    return true;
}

// ----------------------------------------------------------------------------

bool
TcpConnection::connect(
    const InetAddress& destinationIp,
    unsigned short     destinationPort,
    unsigned int       sendingBufferSize,
    unsigned int       receivingBufferSize)
{
    assert(stateM == Idle);

    int              inetFamily;
    sockaddr_storage address;
    socklen_t        addressLen;

    destinationIp.get_socket_address(inetFamily, 
                                     address, 
                                     destinationPort, 
                                     addressLen);

    int fd = socket(((const struct sockaddr*)&address)->sa_family,
                    SOCK_STREAM,
                    0);
    set_socket(fd);

    if(sendingBufferSize != 0)
    {
        set_sending_buffer_size(sendingBufferSize);
    }

    if(receivingBufferSize != 0)
    {
        set_receiving_buffer_size(receivingBufferSize);
    }

    if(!make_non_blocking(get_socket()))
    {
        return false;
    }

    // Connect to remote peer   
    if (connect(get_socket(),
                (const struct sockaddr*)&address,
                addressLen) != 0)
    {
        if (errno == EINPROGRESS)
        {
            // Wait until connect done 
            set_events(EPOLLOUT);
            if (epoll_ctl() == false)
            {
                return false;
            }
            stateM = Connecting;
            return true;
        }
        else
        {
            return false;
        }
   }
   
   stateM = Established;
   return connectionOwnerM->handle_events(this, 0); 
}

bool
TcpConnection::handle_event(
    unsigned int event)
{

}

// ----------------------------------------------------------------------------

bool
TcpConnection::make_non_blocking(
    int socket)
{
    int flags = fcntl(socket, F_GETFL, 0);
    if (fcntl(socket, F_SETFL, flags | O_NONBLOCK) != 0)
    {
       return false;
    }
}

bool
TcpConnection::set_sending_buffer_size(
    unsigned int sendingBufferSize)
{   
    if(setsockopt(get_socket(),
                  SOL_SOCKET,
                  SO_SNDBUF,
                  &sendingBufferSize,
                  sizeof(sendingBufferSize)) != 0)
    {
        return false;
    }

    return true;
}

bool
TcpConnection::set_receiving_buffer_size(
    unsigned int receivingBufferSize)
{
    if(setsockopt(get_socket(),
                  SOL_SOCKET,
                  SO_RCVBUF,
                  &receivingBufferSize,
                  sizeof(receivingBufferSize)) != 0)
    {
        return false;
    }

    return true;
}