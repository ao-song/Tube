#include "EventHandlerTable.h"

using namespace Tube;

// ----------------------------------------------------------------------------

inline
EventHandlerTable::EventHandlerTable()
{
	FdM = epoll_create(EpollSizeE);

	EventsM = new epoll_event[EpollSizeE];

	memset(&EventM, 0, sizeof(EventM));
}

// ----------------------------------------------------------------------------

inline
EventHandlerTable::~EventHandlerTable()
{
	if(FdM != -1)
    {
        close(FdM);
        FdM = -1;
    }

    if(EventsM)
    {
        delete [] EventsM;
    }
}

// ----------------------------------------------------------------------------

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

// ----------------------------------------------------------------------------

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

// ----------------------------------------------------------------------------

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

// ----------------------------------------------------------------------------

bool
EventHandlerTable::handleEvents()
{
   typedef std::set<EventHandler*> EventHandlerSet;
   EventHandlerSet deleteList;

   int numOfEvents = epoll_wait(FdM,
                                EventsM,
                                EpollSizeE,
                                EpollTimeoutE);

   for (int i = 0; i < numOfEvents; ++i)
   {
      EventHandler* eventHandler = 
          static_cast<EventHandler*>(EventsM[i].data.ptr);

      assert(eventHandler != 0);
      if (eventHandler->handle_event(EventsM[i].events) == false)
      {
          eventHandler->remove();
      }
   }

   return true;
}


