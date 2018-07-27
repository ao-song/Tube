#ifndef TCPCONNECTIONOWNER_H
#define TCPCONNECTIONOWNER_H

#include "EventHandlerOwner.h"

namespace Tube
{
    class TcpConnection;
   
    class TcpConnectionOwner : public EventHandlerOwner
    {
    public:
   
        virtual
        ~TcpConnectionOwner() 
        {
            // Empty
        };

        virtual 
        bool
        handle_connect_result(
            TcpConnection* connection) = 0;

        virtual      
        bool
        handle_events(
            TcpConnection* connection,
            unsigned int   events) = 0;
  
        virtual      
        bool
        handle_reset(
            TcpConnection* connection) = 0;
    };
}

#endif