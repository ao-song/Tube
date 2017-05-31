#include "EventHandler.h"

using namespace Tube;

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

// ----------------------------------------------------------------------------

EventHandler::~EventHandler()
{
   close();
}

// ----------------------------------------------------------------------------

bool
EventHandler::epoll_ctl(
	unsigned int previousRequestedEvents)
{
	
}

// ----------------------------------------------------------------------------

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

// ----------------------------------------------------------------------------

inline
EventHandlerTable*
EventHandler::get_table()
{
	return eventHandlerTableM;
}

// ----------------------------------------------------------------------------

inline
int
EventHandler::get_socket() const
{
	return socketM;
}

// ----------------------------------------------------------------------------

inline
EventHandler::set_socket(
    int socket)
{
	assert(socketM == -1);
	socketM = socket;
}

// ----------------------------------------------------------------------------

inline
unsigned int
EventHandler::get_subscribed_events() const
{
	return subscribedEventsM;
}

// ----------------------------------------------------------------------------

inline
void
EventHandler::reset_subscribed_events(
    unsigned int eventsToReset)
{
	subscribedEventsM &= (~eventsToReset);
}

// ----------------------------------------------------------------------------

inline
void
EventHandler::set_subscribed_events(
    unsigned int eventsToSet)
{
	subscribedEventsM |= eventsToSet;
}

// ----------------------------------------------------------------------------

inline
bool
EventHandler::is_want_read() const
{
	return (subscribedEventsM & EPOLLIN) != 0;
}

// ----------------------------------------------------------------------------

inline
bool
EventHandler::is_want_write() const
{
	return (subscribedEventsM & EPOLLOUT) != 0;
}

// ----------------------------------------------------------------------------