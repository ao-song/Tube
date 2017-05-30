#ifndef EVENTHANDLERTABLE_H
#define EVENTHANDLERTABLE_H

// ----------------------------------------------------------------------------

namespace Tube
{
    class EventHandler;

    class EventHandlerTable
    {
    public:
   
        enum 
        {             
            EpollSizeE     = 5000, //any value bigger than 0
            EpollTimeoutE  = 0
        };

        EventHandlerTable();

        ~EventHandlerTable();
      
      
        bool   
        add_event(
            int           socket,
            unsigned int  events,
            EventHandler* eventHandler);
   
    
        bool   
        delete_event(
            int socket);
   
    
        bool   
        modify_event(
            int           socket,
            unsigned int  events,
            EventHandler* eventHandler);
         
     
        bool
        handle_events();
   
    private:
   
        EventHandlerTable(
         const EventHandlerTable& other);
   
        EventHandlerTable&
        operator=(
            const EventHandlerTable& other);
         
        struct epoll_event*  EventsM;
        struct epoll_event   EventM;
        int                  FdM;
    };

// ----------------------------------------------------------------------------   
   
#endif
