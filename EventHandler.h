#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <cassert>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>


namespace Tube
{
    class EventHandlerTable;
    class EventHandlerOwner;

    class EventHandler
    {
    public:
        
        EventHandler(
            EventHandlerTable*  eventHandlerTable,
            EventHandlerOwner*  eventHandlerOwner);

        /**
        * Destructor.
        */
        virtual
        ~EventHandler();

        void
        close();

        void
        set_socket(
            int socket);

        EventHandlerTable*
        get_table();

        
        int
        get_socket() const;

        bool
        set_et();
        
        virtual
        unsigned int
        get_events() const;

        
        virtual
        bool
        handle_event(
            unsigned int event,
            int fd) = 0;

        
    
        virtual
        void
        reset_events(
            unsigned int eventsToReset);

        
        virtual
        bool
        set_events(
            unsigned int eventsToSet);      

        
        virtual
        bool
        is_want_read() const;

        
        virtual
        bool
        is_want_write() const;

    private:

        // Copy constructor not implemented.        
        EventHandler(
            const EventHandler& other);

        // Assignment operator not implemented.     
        EventHandler&
        operator=(
            const EventHandler& other);


        EventHandlerTable*  eventHandlerTableM;
        EventHandlerOwner*  eventHandlerOwnerM;     
        unsigned int        subscribedEventsM;
        int                 socketM;
    };

// ----------------------------------------------------------------------------

    inline
    EventHandler::EventHandler(
        EventHandlerTable*  eventHandlerTable,
        EventHandlerOwner*  eventHandlerOwner)
    :   eventHandlerTableM(eventHandlerTable),
        eventHandlerOwnerM(eventHandlerOwner),    
        subscribedEventsM(0),
        socketM(-1)
    {
        // Empty
    }

    inline
    void
    EventHandler::set_et()
    {
        subscribedEventsM |= EPOLLET;    
    }

    inline
    void
    EventHandler::close()
    {
        if(socketM != -1)
        {
            if(subscribedEventsM != 0)
            {
                eventHandlerTableM->delete_event(socketM);
            }
            ::close(socketM);
            socketM = -1;
        }
    }

    inline
    EventHandlerTable*
    EventHandler::get_table()
    {
        return eventHandlerTableM;
    }

    inline
    int
    EventHandler::get_socket() const
    {
        return socketM;
    }

    inline
    EventHandler::set_socket(
        int socket)
    {
        assert(socketM == -1);
        socketM = socket;
    }

    inline
    unsigned int
    EventHandler::get_events() const
    {
        return subscribedEventsM;
    }

    inline
    void
    EventHandler::reset_events(
        unsigned int eventsToReset)
    {
        subscribedEventsM &= (~eventsToReset);
    }

    inline
    bool
    EventHandler::is_want_read() const
    {
        return (subscribedEventsM & EPOLLIN) != 0;
    }

    inline
    bool
    EventHandler::is_want_write() const
    {
        return (subscribedEventsM & EPOLLOUT) != 0;
    }

}

#endif