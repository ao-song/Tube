#include "EventHandlerTable.h"

using namespace Tube;


// ----------------------------------------------------------------------------

bool
EventHandlerTable::handle_events()
{

   int numOfEvents = epoll_wait(FdM,
                                EventsM,
                                EpollSize,
                                EpollTimeout);

   for (int i = 0; i < numOfEvents; ++i)
   {
      EventHandler* eventHandler = 
          static_cast<EventHandler*>(EventsM[i].data.ptr);
      int fd = EventsM[i].data.fd;

      assert(eventHandler != 0);
      if (eventHandler->handle_event(EventsM[i].events,
                                     fd) == false)
      {
          eventHandler->close();
      }
   }

   return true;
}


