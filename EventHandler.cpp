#include "EventHandler.h"

using namespace Tube;

// ----------------------------------------------------------------------------

EventHandler::~EventHandler()
{
   close();
}

// ----------------------------------------------------------------------------

bool
EventHandler::set_events(
    unsigned int eventsToSet)
{
    unsigned int prevSubscribedEvents = subscribedEventsM;

    subscribedEventsM |= eventsToSet;

    if(prevSubscribedEvents == 0)
    {
        eventHandlerTableM->add_event(socketM,
                                      subscribedEventsM,
                                      this);
    }

    if(subscribedEventsM != 0)
    {
        eventHandlerTableM->modify_event(socketM,
                                         subscribedEventsM,
                                         this);
    }
    else
    {
        eventHandlerTableM->delete_event(socketM);
    }

    return true;
}

