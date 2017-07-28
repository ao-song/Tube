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
    const char*        destinationIp,
    unsigned short     destinationPort,
    unsigned int       sendingBufferSize,
    unsigned int       receivingBufferSize)
{
    assert(stateM == Idle);

    InetAddress destinationAddress;
    destinationAddress.init(destinationIp);
    destinationAddress.set_port(destinationPort);

    const struct sockaddr* address = destinationAddress.get_address();

    int fd = socket(address->sa_family,
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
                address,
                sizeof(address)) != 0)
    {
        if (errno == EINPROGRESS)
        {
            // Wait until connect done 
            set_events(EPOLLOUT);
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
    if(event & (EPOLLHUP | EPOLLERR))
    {
        return connectionOwnerM->handle_reset(this);
    }

    switch(stateM)
    {
        case Connecting:
        {            
            if(event & EPOLLOUT)
            {
                stateM = Established;
                return connectionOwnerM->handle_connect_result(this);
            }

            break;
        }
        case Established:
        {
            return connectionOwnerM->handle_events(this, event);
        }
        default:
        {
            return false;
        }
    }

    return true;
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

Action
TcpConnection::send(
    const void* buffer, 
    size_t      bufferLength)
{
    assert(sendBufferM.is_empty());

    const ssize_t sendResult = send(get_socket(),
                                    buffer,
                                    bufferLength,
                                    0);
 
    if (sendResult < 0)
    {
        if (errno == EAGAIN)
        {
            sendBufferM.append(buffer, bufferLength);
            set_events(EPOLLOUT);
            return WaitForEvent;
        }
        return RemoveConnection;
    }

    const size_t bytesSent = static_cast<const size_t>(sendResult);

    if (bytesSent == bufferLength)
    {
        reset_events(EPOLLOUT);
        return CallAgain;
    }
    else
    {
        sendBufferM.append(buffer,
                           bufferLength,
                           bytesSent);
        set_events(EPOLLOUT);
        return WaitForEvent;
    }
}


void
TcpConnection::reset()
{
    if(get_socket() != -1)
    {
        struct linger so_linger;
        // force exit
        so_linger.l_onoff = 1;
        so_linger.l_linger = 0;

        setsockopt(get_socket(),
                   SOL_SOCKET,
                   SO_LINGER,
                   &so_linger,
                   sizeof(so_linger));
        close();
    }
}

Action
TcpConnection::send_buffered_data()
{
    const size_t bufferLength = sendBufferM.get_length();
    if(bufferLength == 0)
    {
        reset_events(EPOLLOUT);
        return CallAgain;
    }

    const ssize_t sendResult = send(get_socket(),
                                    sendBufferM.get_data(),
                                    bufferLength,
                                    0);

    if (sendResult < 0)
    {
        if (errno == EAGAIN)
        {
           set_events(EPOLLOUT);
           return WaitForEvent;
        }
        return RemoveConnection;
    }

    const size_t bytesSent =
        static_cast<const size_t>(sendResult);

    if(bytesSent == bufferLength)
    {
        assert(sendBufferM.is_empty());
        reset_events(EPOLLOUT);
        return CallAgain;
    }
    else
    {
        set_events(EPOLLOUT);
        return WaitForEvent;
    }
}