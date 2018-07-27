#ifndef EVENTHANDLERTABLE_H
#define EVENTHANDLERTABLE_H

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstring>
#include <unistd.h>
#include <cassert>


// ----------------------------------------------------------------------------

namespace Tube
{
    class EventHandler;

    class EventHandlerTable
    {
    public:
   
        enum 
        {             
            EpollSize     = 5000, //any value bigger than 0
            EpollTimeout  = 0
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

    inline
    EventHandlerTable::EventHandlerTable()
    {
        FdM = epoll_create(EpollSize);

        EventsM = new epoll_event[EpollSize];

        memset(&EventM, 0, sizeof(EventM));
    }

    inline
    EventHandlerTable::~EventHandlerTable()
    {
        if (FdM != -1)
        {
            close(FdM);
            FdM = -1;
        }

        if (EventsM)
        {
            delete [] EventsM;
        }
    }

    inline
    bool
    EventHandlerTable::add_event(
        int           socket,
        unsigned int  events,
        EventHandler* eventHandler)
    {
        EventM.events = events;
        EventM.data.ptr = eventHandler;

        return epoll_ctl(FdM,
                         EPOLL_CTL_ADD,
                         socket,
                         &EventM) == 0;
    }

    inline
    bool
    EventHandlerTable::delete_event(
        int socket)
    {
        return epoll_ctl(FdM,
                         EPOLL_CTL_DEL,
                         socket,
                         &EventM) == 0;
    }

    inline
    bool
    EventHandlerTable::modify_event(
        int           socket,
        unsigned int  events,
        EventHandler* eventHandler)
    {
        EventM.events = events;
        EventM.data.ptr = eventHandler;

        return epoll_ctl(FdM,
                         EPOLL_CTL_MOD,
                         socket,
                         &EventM) == 0;
    }
    
}   
#endif
