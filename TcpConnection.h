#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

namespace Tube
{
    class TcpConnection : public EventHandler
    {
    public:

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
            unsigned int       sendingBufferSize,
            unsigned int       receivingBufferSize);

      

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
   
      TcpConnectionOwner* connectionOwnerM;
      PayloadBuffer       sendBufferM;
      State               stateM;
    };
}

#endif