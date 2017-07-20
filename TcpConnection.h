#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

// ----------------------------------------------------------------------------

namespace Tube
{
    class TcpConnection : public EventHandler
    {
    public:

        typedef enum
        {
            CallAgain,
            RemoveConnection,
            WaitForEvent
        } Action; 

        typedef enum
        {
            Idle,
            Connecting,
            Established
        } State; 

        TcpConnection(
            EventHandlerTable*  eventHandlerTable,
            TcpConnectionOwner* tcpConnectionOwner);
         
      
        virtual
        ~TcpConnection();
      
      
        virtual
        bool
        accept(
            int incomingSocket);

      
        virtual
        bool
        connect(
            const InetAddress& destinationIp,
            unsigned short     destinationPort,
            unsigned int       sendingBufferSize = 0,
            unsigned int       receivingBufferSize = 0);
      

        virtual
        void
        fin();


        virtual
        TcpConnectionOwner*
        get_connection_owner();      
   
         
      
        virtual
        bool
        handle_event(
            unsigned int event);      

      
        bool
        is_established() const;
         
    
        bool
        is_ok_to_send() const;

        virtual
        Action
        send(
            const void* buffer, 
            size_t      bufferLength);
      
      
        virtual
        ssize_t
        receive(
            void*  buffer, 
            size_t bufferLength);
         

        void
        reset();        
         

        virtual
        Action
        send_buffered_data();

         
   private:
   
      /**
       * Copy constructor not implemented.
       */   
      TcpConnection(
         const TcpConnection& other);

      /**
       * Assignment operator not implemented.
       */
      TcpConnection&
      operator=(
         const TcpConnection& other);

      bool
      make_non_blocking(
          int socket);

      bool
      set_sending_buffer_size(
          unsigned int sendingBufferSize);

      bool
      set_receiving_buffer_size(
          unsigned int receivingBufferSize);
   
      TcpConnectionOwner* connectionOwnerM;
      PayloadBuffer       sendBufferM;
      State               stateM;
    };
}

inline
void
TcpConnection::fin()
{
    shutdown(get_socket(), SHUT_WR);
}

inline
TcpConnectionOwner*
TcpConnection::get_connection_owner()
{
    return connectionOwnerM;
}

#endif