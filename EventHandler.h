#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

namespace Tube
{
	class EventHandler
	{
	public:

		/**
		* Constructor.
		* @param theEventHandlerTable  a pointer to the EventHandlerTable object
		*                              to which this EventHandler is connected
		*                              to (must not be 0!).
		* @param theEventHandlerParent a pointer to the object (derived from
		*                              EventHandlerParent) to which this
		*                              EventHandler is connected to (may be 0).
		*/
		EventHandler(
		 EventHandlerTable*  theEventHandlerTable,
		 EventHandlerParent* theEventHandlerParent);

		/**
		* Destructor.
		*/
		virtual
		~EventHandler();

		/**
		* Close the socket to which this event handler is connected to.
		* The EventHandler's destructor automatically calls this method.
		*/
		void
		close();

		/**
		* Subscribe to new events or cancel the subscription to previous events.
		* The method compares the previously requested events
		* (thePreviousRequestedEvents) with the currently requested events
		* (manipulated with setRequestedEvents() and resetRequestedEvents()).
		* Depending on the result of that comparison, the method adds the socket
		* to the event handler table(EPOLL_CTL_ADD), removes it from the
		* event handler table (EPOLL_CTL_DEL), or modifies the subscribed
		* events (EPOLL_CTL_MOD).
		* If there is no difference between the previously requested events and
		* the currently requested events then the method does nothing.
		* @param previousRequestedEvents a bit field, which contains the
		*                                   previously requested events:
		*                                   EPOLLIN  the socket is available
		*                                                for read operations
		*                                   EPOLLOUT the socket is available
		*                                                for write operations
		*/
		virtual
		bool
		epoll_ctl(unsigned int previousRequestedEvents = 0);

		/**
		* Get a pointer to the object (derived from eventHandlerOwner) to which
		* this EventHandler is connected to (may be 0).
		* @return a pointer to the eventHandlerOwner or 0 if this eventHandler
		*         is not connected to any eventHandlerOwner.
		*/
		EventHandlerOwner*
		get_owner();

		/**
		* Set the EventHandlerParent
		*/
		void
		set_owner(eventHandlerOwner* theEventHandlerOwner);

		/**
		* Get a pointer to the eventHandlerTable object to which this
		* eventHandler is connected to.
		* @return a pointer to the eventHandlerTable
		*/
		EventHandlerTable*
		get_table();

		/**
		* Get the socket, to which this EventHandler is connected to.
		* @return the socket to which this EventHandler is connected to.
		*/
		int
		get_socket() const;

		/**
		* Get the events to which this EventHandler wants to 'subscribe' to
		* (requested events).
		* @return a bit field, which contains the requested events:
		*         EPOLLIN  the socket is available for read operations
		*         EPOLLOUT the socket is available for write operation
		*/
		virtual
		unsigned int
		get_request_events() const;

		/**
		* Handle the events, which have occured on the socket.
		* Classes, which inherit from the EventHandler class must implement this
		* method.
		* @param theEvent a bit field, which contains the occured events:
		*                 EPOLLIN  the socket is available for read operations
		*                 EPOLLOUT the socket is available for write operation
		*                 EPOLLERR an error condition happend on the socket
		*                 EPOLLHUP hang up happened on the socket
		* @return 'true'  if the EventHandler should be kept after the call,
		*         'false' if the EventHandler should be deleted after the call.
		*/
		virtual
		bool
		handle_event(
		 unsigned int theEvent) = 0;

		/**
		* Insert the local address of this EventHandler into the the specified
		* stream.
		* @param theStream    the stream
		* @return the stream
		*/
		void
		local_address_to_stream(
		 std::ostream& theStream) const;

		/**
		* Insert the remote address of this EventHandler into the the specified
		* stream.
		* @param theStream    the stream
		* @return the stream
		*/
		void
		remote_address_to_stream(
		 std::ostream& theStream) const;

		/**
		* Reset (clear) the specified requested events.
		* @param theEventsToReset a bit field, which contains the events that
		*                         should be reset (cleared)
		*         EPOLLIN  the socket is available for read operations
		*         EPOLLOUT the socket is available for write operation
		*/
		virtual
		void
		reset_requested_events(
		 unsigned int theEventsToReset);

		/**
		* Set the specified requested events.
		* @param theEventsToSet a bit field, which contains the events that
		*                       should be set
		*         EPOLLIN  the socket is available for read operations
		*         EPOLLOUT the socket is available for write operation
		*/
		virtual
		void
		set_requested_events(
		 unsigned int theEventsToSet);

		/**
		* Set the socket, to which the event handler is connected.
		* IMPORTANT: This method can only be called once!
		* @param theSocket the socket to which the event handler is connected.
		*/
		void
		set_socket(
		 int theSocket);

		/**
		* Check if this EventHandler wants to read something on the socket
		* @return 'true'  if this EventHandler wants to read something
		*                 (EPOLLIN bit is set)
		*         'false' otherwise
		*/
		virtual
		bool
		is_want_read() const;

		/**
		* Check if this EventHandler wants to write something on the socket
		* @return 'true'  if the EventHandler wants to write something
		*                 (EPOLLOUT bit is set)
		*         'false' otherwise
		*/
		virtual
		bool
		is_want_write() const;

		void
		change_event_handler_table(EventHandlerTable* theTable);

		virtual
		void
		remove () = 0;

	private:

		/**
		* Copy constructor not implemented.
		*/
		EventHandler(
		 const EventHandler& theOther);

		/**
		* Assignment operator not implemented.
		*/
		EventHandler&
		operator=(
		 const EventHandler& theOther);

		void
		address_tostream_impl(
		 std::ostream& theStream,
		 const sockaddr_storage&,
		 socklen_t&) const;

		EventHandlerOwner* eventHandlerOwnerM;
		EventHandlerTable*  eventHandlerTableM;
		unsigned int        requestedEventsM;
		int                 socketM;
	};
}

#endif