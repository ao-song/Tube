#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <cassert>
#include <sys/epoll.h>


namespace Tube
{
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

        
        virtual
        bool
        epoll_ctl(
            unsigned int previousRequestedEvents = 0);      

        
        virtual
        unsigned int
        get_events() const;

        
        virtual
        bool
        handle_event(
            unsigned int event) = 0;

        
    
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

        virtual
        void
        remove () = 0;

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
}

#endif